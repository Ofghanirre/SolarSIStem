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
        float size,
        Context ctxtCircle
    ){
        use();
        glm::mat4 planetMVMatrix = glm::scale(globalMVMatrix, glm::vec3(size / 2, size / 2, size / 2)); // Translation * Rotation * Translation * Scale

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(planetMVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(planetMVMatrix))));
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(ctxtCircle.ProjMatrix * planetMVMatrix));

        glBindVertexArray(ctxtCircle.vao); // On utilise l'array vao
        glDrawArrays(GL_LINES, 0, ctxtCircle.m_circle->getVertexCount());
        glBindVertexArray(0); // On utilise l'array vao
    }
};
