#pragma once

#include "libs.hpp"
#include "utils.hpp"
#include "rings.hpp"
#include "context.hpp"
#include <glimac/Sphere.hpp>
#include <iostream>
#include <cassert>

struct SkyBox {
    Program &m_Program;

    GLuint skyboxVAO, skyboxVBO;
    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;
    GLuint m_textureID;
    GLuint m_uTexture;

    SkyBox(Program& program, std::vector<glimac::FilePath> faces) : 
        m_Program{program}
    {
        m_Program.use();
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "projection"), "projection");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "view"), "view");
        m_uTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "skybox"), "skybox");
        loadCubemap(faces);
        std::cout << m_uMVPMatrix << " " << m_uMVMatrix << " " << m_uTexture << " " << m_textureID << std::endl;
    }

    void loadCubemap(std::vector<glimac::FilePath> faces)
    {
        glEnable(GL_TEXTURE_CUBE_MAP);
        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
            for (unsigned int i = 0; i < faces.size(); i++)
            {
                std::unique_ptr<Image> loaded_content = loadImage(faces[i]);
                if (loaded_content)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, loaded_content.get()->getWidth(), loaded_content.get()->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loaded_content.get()->getPixels());
                } else {
                    std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void use() {
        m_Program.use();
    }

    void setContext() {
        glGenBuffers(1, &skyboxVBO);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &skyboxVAO);
        glBindVertexArray(skyboxVAO);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
                glVertexAttribPointer(
                    0, 
                    3, 
                    GL_FLOAT, 
                    GL_FALSE, 
                    3 * sizeof(float), 
                    (void*)0
                );
            glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind le buffer vbo
        glBindVertexArray(0); // On utilise l'array vao
    }

    void free() {
        glDeleteBuffers(1, &skyboxVBO);
        glDeleteVertexArrays(1, &skyboxVAO);
    }

    void draw(
        glm::mat4 viewMatrix
    ) {
        glEnable(GL_TEXTURE_CUBE_MAP);
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        use();
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(m_uTexture, 0);

        glm::mat4 view = glm::mat4(glm::mat3(viewMatrix)); // remove translation from the view matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.f), WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.f);
        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projection));

        
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 30);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    }

    const std::vector<float> skyboxVertices = {
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
};