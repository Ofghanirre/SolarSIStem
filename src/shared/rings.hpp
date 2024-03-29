#pragma once

#include "libs.hpp"
#include "utils.hpp"
#include "context.hpp"
#include <glimac/Circle.hpp>
#include <iostream>
#include <cassert>

using namespace glimac;


struct RingsObject {
    Program &m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;

    RingsObject(Program& program) : 
        m_Program{program}
    {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
    }

    void update() {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
    }

    void use() {
        m_Program.use();
    }

    void draw(
        glm::mat4 globalMVMatrix, 
        float width,
        float height,
        Context<Circle> ctxtShape
    ){
        use();
        glm::mat4 planetMVMatrix = glm::scale(globalMVMatrix, glm::vec3(width / 2, height / 2, height / 2)); // Translation * Rotation * Translation * Scale

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(planetMVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(planetMVMatrix))));
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(ctxtShape.ProjMatrix * planetMVMatrix));

        glBindVertexArray(ctxtShape.vao); // On utilise l'array vao
        glDrawArrays(GL_LINES, 0, ctxtShape.m_shape->getVertexCount());
        glBindVertexArray(0); // On utilise l'array vao
    }
};


struct FilledRingsObject {
    Program &m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;

    GLint m_texture_id;
    GLint m_uLightSourceLocation;
    GLuint m_isLightOnLocation;

    FilledRingsObject(Program& program) : 
        m_Program{program}
    {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        m_texture_id = checkValid(glGetUniformLocation(m_Program.getGLId(), "uRingTexture"), "uRingTexture");
        m_uLightSourceLocation = glGetUniformLocation(m_Program.getGLId(), "uLightSource");
        m_isLightOnLocation = glGetUniformLocation(m_Program.getGLId(), "isLightOn");
    }

    void update() {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        m_texture_id = checkValid(glGetUniformLocation(m_Program.getGLId(), "uRingTexture"), "uRingTexture");
        m_uLightSourceLocation = glGetUniformLocation(m_Program.getGLId(), "uLightSource");
        m_isLightOnLocation = glGetUniformLocation(m_Program.getGLId(), "isLightOn");
    }

    void use() {
        m_Program.use();
    }

    void draw(
        glm::mat4 globalMVMatrix, 
        float size,
        Context<Ring> ctxtRing,
        GLuint texture_id,
        glm::vec3 lightSourcePosition,
        int isLightOn
    ){
        use();
        glUniform1i(m_texture_id, 0);
        glm::mat4 planetMVMatrix = glm::scale(globalMVMatrix, glm::vec3(size / 2, size / 2, size / 2)); // Translation * Rotation * Translation * Scale

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(planetMVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(planetMVMatrix))));
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(ctxtRing.ProjMatrix * planetMVMatrix));

        glUniform3f(m_uLightSourceLocation, lightSourcePosition.x, lightSourcePosition.y, lightSourcePosition.z);
        glUniform1i(m_isLightOnLocation, isLightOn);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        
        glBindVertexArray(ctxtRing.vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, ctxtRing.m_shape->getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
    }
};