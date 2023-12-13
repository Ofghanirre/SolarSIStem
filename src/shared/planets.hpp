#include "libs.hpp"
#include "utils.hpp"
#include <glimac/Sphere.hpp>
#include <iostream>

using namespace glimac;


struct AStellarObject {
    Program m_Program;

    const float m_coeff_diametre;
    const float m_dist_sol;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;
    std::vector<GLuint> m_textures;
    std::vector<const GLchar*> m_textures_name;
    glm::vec3 m_rotation_axis;
    glm::vec3 m_initial_position;

    AStellarObject(const FilePath& applicationPath, 
                    float coeff_diametre, 
                    float dist_sol,
                    std::vector<const GLchar*> textures_uniform_locations,
                    glm::vec3 rotation_axis,
                    glm::vec3 initial_position
        ) : m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                                  applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")), 
            m_coeff_diametre{coeff_diametre},
            m_dist_sol{dist_sol},
            m_rotation_axis{rotation_axis},
            m_initial_position{initial_position}
    {
        for (const auto& iter_tex_uniform_location : textures_uniform_locations) {
            m_textures_name.push_back(iter_tex_uniform_location);
            m_textures.push_back(checkValid(glGetUniformLocation(m_Program.getGLId(), 
                iter_tex_uniform_location), iter_tex_uniform_location));
        }
    }


    void update() {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");

        for (int i = 0; m_textures_name.size(); i++) {
            m_textures[i] = checkValid(glGetUniformLocation(m_Program.getGLId(), 
                m_textures_name[i]), m_textures_name[i]);
        }
    }

    void use() {
        m_Program.use();
    }

    virtual void draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time, 
        std::vector<GLuint> TEXTURES_ID, 
        GLuint vao, 
        Sphere sphere) 
    {
        use();
        
        glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
        glm::mat4 objectMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, m_rotation_axis); // Translation * Rotation
        objectMVMatrix = glm::translate(objectMVMatrix, m_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = objectMVMatrix;
        objectMVMatrix = glm::scale(objectMVMatrix, glm::vec3(m_coeff_diametre, m_coeff_diametre, m_coeff_diametre)); // Translation * Rotation * Translation * Scale
        objectMVMatrix = glm::rotate(objectMVMatrix, time, m_rotation_axis); // Translation * Rotation

        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(objectMVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(objectMVMatrix))));
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * objectMVMatrix));

        // Textures Loading
        for (int i = 0; i < TEXTURES_ID.size(); i++) {
            glUniform1i(TEXTURES_ID[i], i);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, i);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
        std::cout << objectMVMatrix << std::endl;
    }
};

struct SunProgram {
    Program m_Program;

    float coef_diametre = 0.7;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uSunTexture;

    SunProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uSunTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uSunTexture"), "uSunTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uSunTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uSunTexture"), "uSunTexture");
    }

    void use() {
        m_Program.use();
    }

    void draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint SUN_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
        glUniform1i(uSunTexture, 0);

        glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
        glm::mat4 sunMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
        sunMVMatrix = glm::scale(sunMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(sunMVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(sunMVMatrix))));
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * sunMVMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, SUN_TEXTURE_ID);

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
    }
};

struct MercureProgram : public AStellarObject {
    const float coef_diametre = 0.02439;
    const float dist_sol = 2.06;

    MercureProgram(const FilePath& applicationPath): AStellarObject{
        applicationPath, 
        coef_diametre, 
        dist_sol, 
        {"uMercureTexture"},
        glm::vec3(0, 1, 0), 
        glm::vec3(dist_sol, 0, 0), 
        } 
    {}
};

struct VenusProgram {
    Program m_Program;

    float coef_diametre = 0.0605;
    float dist_sol = 3.86;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uVenusTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    VenusProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uVenusTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uVenusTexture"), "uVenusTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uVenusTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uVenusTexture"), "uVenusTexture");
    }

    void use() {
        m_Program.use();
    }

    glm::mat4 draw(
            glm::mat4 globalMVMatrix, 
            glm::mat4 viewMatrix, 
            glm::mat4 ProjMatrix, 
                    
            float time, 
            GLuint VENUS_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uVenusTexture, 0);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 venusMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            venusMVMatrix = glm::translate(venusMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = venusMVMatrix;
            venusMVMatrix = glm::scale(venusMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            venusMVMatrix = glm::rotate(venusMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(venusMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(venusMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * venusMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, VENUS_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
            return MVMatrixPos;
    }
};

struct EarthProgram {
    Program m_Program;

    float coef_diametre = 0.06376;
    float dist_sol = 5.1675;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

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

    glm::mat4 draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint EARTH_TEXTURE_ID, GLuint CLOUD_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uEarthTexture, 0);
            glUniform1i(uCloudTexture, 1);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            earthMVMatrix = glm::translate(earthMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = earthMVMatrix;
            earthMVMatrix = glm::scale(earthMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            earthMVMatrix = glm::rotate(earthMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
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
            return MVMatrixPos;
    }
};

struct MoonProgram {
    Program m_Program;

    float coef_diametre = 0.01737;
    float dist_earth = 0.03918;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_earth, 0, 0); //glm::sphericalRand(2.f);

    MoonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
        uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
        std::cout << sattelites_initial_position << std::endl;
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
        glm::mat4 MVMatrixSattelite = globalMVMatrix ;//* viewMatrix; // Translation
        MVMatrixSattelite = glm::rotate(MVMatrixSattelite, time/2, sattelites_rotation_axis); // Translation * Rotation
        MVMatrixSattelite = glm::translate(MVMatrixSattelite, sattelites_initial_position); // Translation * Rotation * Translation
        MVMatrixSattelite = glm::scale(MVMatrixSattelite, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        MVMatrixSattelite = glm::rotate(MVMatrixSattelite, time, sattelites_rotation_axis); // Translation * Rotation
        glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix * MVMatrixSattelite));
        glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(MVMatrixSattelite));
        glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindVertexArray(0); // On réinitialise l'array vao
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

struct MarsProgram {
    Program m_Program;

    float coef_diametre = 0.0339;
    float dist_sol = 8.14;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uMarsTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    MarsProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uMarsTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMarsTexture"), "uMarsTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uMarsTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMarsTexture"), "uMarsTexture");
    }

    void use() {
        m_Program.use();
    }

    glm::mat4 draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint MARS_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uMarsTexture, 0);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 marsMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            marsMVMatrix = glm::translate(marsMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = marsMVMatrix;
            marsMVMatrix = glm::scale(marsMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            marsMVMatrix = glm::rotate(marsMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(marsMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(marsMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * marsMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, MARS_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
            return MVMatrixPos;
    }
};

struct JupiterProgram {
    Program m_Program;

    float coef_diametre = 0.7149;
    float dist_sol = 27.80;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uJupiterTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    JupiterProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uJupiterTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uJupiterTexture"), "uJupiterTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uJupiterTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uJupiterTexture"), "uJupiterTexture");
    }

    void use() {
        m_Program.use();
    }

    glm::mat4 draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint JUPITER_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uJupiterTexture, 0);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 jupiterMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            jupiterMVMatrix = glm::translate(jupiterMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = jupiterMVMatrix;
            jupiterMVMatrix = glm::scale(jupiterMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            jupiterMVMatrix = glm::rotate(jupiterMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(jupiterMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(jupiterMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * jupiterMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, JUPITER_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
            return MVMatrixPos;
    }
};

struct SaturneProgram {
    Program m_Program;

    float coef_diametre = 0.6026;
    float dist_sol = 51.14;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uSaturneTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    SaturneProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uSaturneTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uSaturneTexture"), "uSaturneTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uSaturneTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uSaturneTexture"), "uSaturneTexture");
    }

    void use() {
        m_Program.use();
    }

    glm::mat4 draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint SATURNE_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uSaturneTexture, 0);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 saturneMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            saturneMVMatrix = glm::translate(saturneMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = saturneMVMatrix;
            saturneMVMatrix = glm::scale(saturneMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            saturneMVMatrix = glm::rotate(saturneMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(saturneMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(saturneMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * saturneMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, SATURNE_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
            return MVMatrixPos;
    }
};

struct UranusProgram {
    Program m_Program;

    float coef_diametre = 0.2555;
    float dist_sol = 102.39;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uUranusTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    UranusProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uUranusTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uUranusTexture"), "uUranusTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uUranusTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uUranusTexture"), "uUranusTexture");
    }

    void use() {
        m_Program.use();
    }

    glm::mat4 draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint URANUS_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uUranusTexture, 0);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 uranusMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            uranusMVMatrix = glm::translate(uranusMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = uranusMVMatrix;
            uranusMVMatrix = glm::scale(uranusMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            uranusMVMatrix = glm::rotate(uranusMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(uranusMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(uranusMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * uranusMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, URANUS_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
            return MVMatrixPos;
    }
};

struct NeptuneProgram {
    Program m_Program;

    float coef_diametre = 0.2476;
    float dist_sol = 161.25;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uNeptuneTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    NeptuneProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uNeptuneTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNeptuneTexture"), "uNeptuneTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uNeptuneTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNeptuneTexture"), "uNeptuneTexture");
    }

    void use() {
        m_Program.use();
    }

    glm::mat4 draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint NEPTUNE_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uNeptuneTexture, 0);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 neptuneMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            neptuneMVMatrix = glm::translate(neptuneMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = neptuneMVMatrix;
            neptuneMVMatrix = glm::scale(neptuneMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            neptuneMVMatrix = glm::rotate(neptuneMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(neptuneMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(neptuneMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * neptuneMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, NEPTUNE_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
            return MVMatrixPos;
    }
};

struct PlutonProgram {
    Program m_Program;

    float coef_diametre = 0.0118;
    float dist_sol = 210.94;
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uPlutonTexture;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    PlutonProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uPlutonTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uPlutonTexture"), "uPlutonTexture");
    }

    void update() {
        uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        uPlutonTexture = checkValid(glGetUniformLocation(m_Program.getGLId(), "uPlutonTexture"), "uPlutonTexture");
    }

    void use() {
        m_Program.use();
    }

    glm::mat4 draw(glm::mat4 globalMVMatrix, glm::mat4 viewMatrix, glm::mat4 ProjMatrix, 
            float time, GLuint PLUTON_TEXTURE_ID, GLuint vao, Sphere sphere) {
        use();
            glUniform1i(uPlutonTexture, 0);

            glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
            //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
            glm::mat4 plutonMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
            plutonMVMatrix = glm::translate(plutonMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
            glm::mat4 MVMatrixPos = plutonMVMatrix;
            plutonMVMatrix = glm::scale(plutonMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
            plutonMVMatrix = glm::rotate(plutonMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
            
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(plutonMVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(plutonMVMatrix))));
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * plutonMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, PLUTON_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao
            return MVMatrixPos;
    }
};

