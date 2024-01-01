#include "libs.hpp"
#include "utils.hpp"
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

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time, 
        GLuint vao, 
        Circle circle
    ){
        use();
        glm::mat4 planetMVMatrix = glm::scale(globalMVMatrix, glm::vec3(3, 3, 3)); // Translation * Rotation * Translation * Scale

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(planetMVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(planetMVMatrix))));
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(ProjMatrix * planetMVMatrix));

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_LINES, 0, circle.getVertexCount());
        glBindVertexArray(0); // On utilise l'array vao
    }
};
