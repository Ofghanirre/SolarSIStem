#include "libs.hpp"
#include "utils.hpp"
#include "shaders.hpp"
#include <glimac/Sphere.hpp>
#include <iostream>
#include <cassert>
#include <unistd.h>

using namespace glimac;

struct SkyBox {
    GLint textureID;
    GLint m_texture;
    GLint BACKGROUND_TEXTURE_ID;
    Program *m_Program;
    GLint m_uMVPMatrix;
    GLint m_uViewMatrix;
    std::vector<float> skyboxVertices = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    SkyBox(Program* program) : m_Program{program} {
        BACKGROUND_TEXTURE_ID = registerNewBoxTexture("../assets/textures/Stars.png");
        m_Program->use();
        auto idProg = m_Program->getGLId();
        m_uMVPMatrix = checkValid(glGetUniformLocation(idProg, "uMVPMatrix"), "uMVPMatrix");
        m_uViewMatrix = checkValid(glGetUniformLocation(idProg, "uViewMatrix"), "uViewMatrix");
        m_texture = checkValid(glGetUniformLocation(idProg, "uSkyBox"), "uSkyBox");
    }

    void draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        GLuint vao
    ) {
        glDepthFunc(GL_LEQUAL);
        m_Program->use();
        glUniform1i(m_texture, BACKGROUND_TEXTURE_ID);
        glUniformMatrix4fv(SkyBox::m_uViewMatrix, 1, GL_FALSE, 
            glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(SkyBox::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, BACKGROUND_TEXTURE_ID);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }
};