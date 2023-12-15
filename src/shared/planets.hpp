#include "libs.hpp"
#include "utils.hpp"
#include <glimac/Sphere.hpp>
#include <iostream>
#include <cassert>

using namespace glimac;


struct AStellarObject {
    Program &m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;
    std::vector<GLuint> m_textures;
    std::vector<const GLchar*> m_texturesNames;
    std::vector<GLuint> m_texturesIds;
    std::vector<GLuint> ArchiveTextureName = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};
    std::vector<AStellarObject*> m_satelites;

    AStellarObject(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds) : 
        m_Program{program}, m_textures{texturesIds}
    {
        assert(textures_uniform_locations.size() < 3);
        assert(textures_uniform_locations.size() == texturesIds.size());
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        for (uint i = 0; i < texturesIds.size(); i++){
            m_texturesIds.emplace_back(texturesIds[i]);
            m_texturesNames.emplace_back(textures_uniform_locations[i]);
            m_textures.emplace_back(checkValid(glGetUniformLocation(m_Program.getGLId(), textures_uniform_locations[i]), textures_uniform_locations[i]));
        }
    }

    void update() {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        for (auto textureName: m_texturesNames){
            m_textures.emplace_back(checkValid(glGetUniformLocation(m_Program.getGLId(), textureName), textureName));
        }
    }
    
    
    void addSatelite(AStellarObject* satelite) {
        m_satelites.emplace_back(satelite);
    }

    
    void drawAll(glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time,
        GLuint vao, 
        Sphere sphere
    ) {
        glm::mat4 matrixPos = draw(globalMVMatrix, viewMatrix, ProjMatrix, time, vao, sphere);
        for(auto satelite : m_satelites){
            satelite->drawAll(matrixPos, viewMatrix, ProjMatrix, time, vao, sphere);
        }
    }

    void use() {
        m_Program.use();
    }

    virtual glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time, 
        GLuint vao, 
        Sphere sphere) = 0;
};


struct SunProgram : public AStellarObject{
    float coef_diametre = 0.7;
    const float dist_sol = 0.0f;

    SunProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
        glm::mat4 sunMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
        sunMVMatrix = glm::scale(sunMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre));
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(sunMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(sunMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * sunMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
        return globalMVMatrix;
    }
};


struct MercureProgram : public AStellarObject {
    const float coef_diametre = 0.02439;
    const float dist_sol = 2.06;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    MercureProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time,
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
        //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
        glm::mat4 mercureMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
        mercureMVMatrix = glm::translate(mercureMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = mercureMVMatrix;
        mercureMVMatrix = glm::scale(mercureMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        mercureMVMatrix = glm::rotate(mercureMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation

        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(mercureMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(mercureMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * mercureMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
        return MVMatrixPos;
    }
};


struct VenusProgram : public AStellarObject {
    float coef_diametre = 0.0605;
    float dist_sol = 3.86;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    VenusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time,
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
        //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
        glm::mat4 venusMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
        venusMVMatrix = glm::translate(venusMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = venusMVMatrix;
        venusMVMatrix = glm::scale(venusMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        venusMVMatrix = glm::rotate(venusMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(venusMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(venusMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * venusMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
        return MVMatrixPos;
    }
};


struct EarthProgram : public AStellarObject {
    float coef_diametre = 0.06376;
    float dist_sol = 5.1675;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    EarthProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time,
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
        //glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time, glm::vec3(0, 1, 0));
        glm::mat4 earthMVMatrix = glm::rotate(MVMatrixPlanetView, time/2, sattelites_rotation_axis); // Translation * Rotation
        earthMVMatrix = glm::translate(earthMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = earthMVMatrix;
        earthMVMatrix = glm::scale(earthMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        earthMVMatrix = glm::rotate(earthMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation

        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(earthMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * earthMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
        return MVMatrixPos;
    }
};


struct MoonProgram : public AStellarObject {
    float coef_diametre = 0.01737;
    float dist_earth = 0.03918 + 0.0605;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_earth, 0, 0); //glm::sphericalRand(2.f);

    MoonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix,
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 MVMatrixSattelite = globalMVMatrix ;//* viewMatrix; // Translation
        MVMatrixSattelite = glm::rotate(MVMatrixSattelite, time/2, sattelites_rotation_axis); // Translation * Rotation
        MVMatrixSattelite = glm::translate(MVMatrixSattelite, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = MVMatrixSattelite;
        MVMatrixSattelite = glm::scale(MVMatrixSattelite, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        MVMatrixSattelite = glm::rotate(MVMatrixSattelite, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(MVMatrixSattelite));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(MVMatrixSattelite))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * MVMatrixSattelite));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On réinitialise l'array vao
        return MVMatrixPos;
    }
};


struct MarsProgram : public AStellarObject {
    float coef_diametre = 0.0339;
    float dist_sol = 8.14;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0);

    MarsProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix,
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 marsMVMatrix = globalMVMatrix * viewMatrix; // Translation
        marsMVMatrix = glm::rotate(marsMVMatrix, time/2, sattelites_rotation_axis); // Translation * Rotation
        marsMVMatrix = glm::translate(marsMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = marsMVMatrix;
        marsMVMatrix = glm::scale(marsMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        marsMVMatrix = glm::rotate(marsMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(marsMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(marsMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * marsMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On réinitialise l'array vao
        return MVMatrixPos;
    }
};


struct JupiterProgram : public AStellarObject {
    float coef_diametre = 0.7149;
    float dist_sol = 27.80;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    JupiterProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix,
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 jupiterMVMatrix = globalMVMatrix * viewMatrix; // Translation
        jupiterMVMatrix = glm::rotate(jupiterMVMatrix, time/2, sattelites_rotation_axis); // Translation * Rotation
        jupiterMVMatrix = glm::translate(jupiterMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = jupiterMVMatrix;
        jupiterMVMatrix = glm::scale(jupiterMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        jupiterMVMatrix = glm::rotate(jupiterMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(jupiterMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(jupiterMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * jupiterMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On réinitialise l'array vao
        return MVMatrixPos;
    }
};

struct SaturneProgram : public AStellarObject {
    float coef_diametre = 0.6026;
    float dist_sol = 51.14;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    SaturneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix,
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 saturneMVMatrix = globalMVMatrix * viewMatrix; // Translation
        saturneMVMatrix = glm::rotate(saturneMVMatrix, time/2, sattelites_rotation_axis); // Translation * Rotation
        saturneMVMatrix = glm::translate(saturneMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = saturneMVMatrix;
        saturneMVMatrix = glm::scale(saturneMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        saturneMVMatrix = glm::rotate(saturneMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(saturneMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(saturneMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * saturneMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On réinitialise l'array vao
        return MVMatrixPos;
    }
};

struct UranusProgram : public AStellarObject {
    float coef_diametre = 0.2555;
    float dist_sol = 102.39;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    UranusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix,
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 uranusMVMatrix = globalMVMatrix * viewMatrix; // Translation
        uranusMVMatrix = glm::rotate(uranusMVMatrix, time/2, sattelites_rotation_axis); // Translation * Rotation
        uranusMVMatrix = glm::translate(uranusMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = uranusMVMatrix;
        uranusMVMatrix = glm::scale(uranusMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        uranusMVMatrix = glm::rotate(uranusMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(uranusMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(uranusMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * uranusMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On réinitialise l'array vao
        return MVMatrixPos;
    }
};

struct NeptuneProgram : public AStellarObject {
    float coef_diametre = 0.2476;
    float dist_sol = 161.25;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    NeptuneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix,
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 neptuneMVMatrix = globalMVMatrix * viewMatrix; // Translation
        neptuneMVMatrix = glm::rotate(neptuneMVMatrix, time/2, sattelites_rotation_axis); // Translation * Rotation
        neptuneMVMatrix = glm::translate(neptuneMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = neptuneMVMatrix;
        neptuneMVMatrix = glm::scale(neptuneMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        neptuneMVMatrix = glm::rotate(neptuneMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(neptuneMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(neptuneMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * neptuneMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On réinitialise l'array vao
        return MVMatrixPos;
    }
};

struct PlutonProgram : public AStellarObject {
    float coef_diametre = 0.0118;
    float dist_sol = 210.94;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_sol, 0, 0); //glm::sphericalRand(2.f);

    PlutonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds}
    {}

    glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix,
        float time, 
        GLuint vao, 
        Sphere sphere) override
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 plutonMVMatrix = globalMVMatrix * viewMatrix; // Translation
        plutonMVMatrix = glm::rotate(plutonMVMatrix, time/2, sattelites_rotation_axis); // Translation * Rotation
        plutonMVMatrix = glm::translate(plutonMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = plutonMVMatrix;
        plutonMVMatrix = glm::scale(plutonMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        plutonMVMatrix = glm::rotate(plutonMVMatrix, time, sattelites_rotation_axis); // Translation * Rotation
        
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE, 
            glm::value_ptr(plutonMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE, 
            glm::value_ptr(glm::transpose(glm::inverse(plutonMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE, 
            glm::value_ptr(ProjMatrix * plutonMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On réinitialise l'array vao
        return MVMatrixPos;
    }
};

