#include "libs.hpp"
#include "utils.hpp"
#include <glimac/Sphere.hpp>

using namespace glimac;

struct EarthProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uEarthTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uEarthTexture"), "uEarthTexture");
        uCloudTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uCloudTexture"), "uCloudTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uEarthTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uEarthTexture"), "uEarthTexture");
        uCloudTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uCloudTexture"), "uCloudTexture");
    }

    void use() {
        m_Program.use();
    }

    void draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint EARTH_TEXTURE_ID, GLuint CLOUD_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uEarthTexture, 0);
            glUniform1i(uCloudTexture, 1);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(earthMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * earthMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, EARTH_TEXTURE_ID);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, CLOUD_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
    }
};

struct MoonProgram {
    Program m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;
    glm::vec3 sattelites_rotation_axis = glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::sphericalRand(2.f);

    MoonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
    }

    void use() {
        m_Program.use();
    }

    void draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, 
            float time, GLuint MOON_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
        glUniform1i(uTexture, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MOON_TEXTURE_ID);
        glBindVertexArray(vao); // On utilise l'array vao
        glm::mat4 MVMatrixSattelite = globalMVMatrix * viewMatrix; // Translation
        MVMatrixSattelite = glm::rotate(MVMatrixSattelite, time/2, sattelites_rotation_axis); // Translation * Rotation
        MVMatrixSattelite = glm::translate(MVMatrixSattelite, sattelites_initial_position); // Translation * Rotation * Translation
        MVMatrixSattelite = glm::scale(MVMatrixSattelite, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
        MVMatrixSattelite = glm::rotate(MVMatrixSattelite, time, sattelites_rotation_axis); // Translation * Rotation
        glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix * MVMatrixSattelite));
        glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(MVMatrixSattelite));
        glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindVertexArray(0); // On réinitialise l'array vao
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};