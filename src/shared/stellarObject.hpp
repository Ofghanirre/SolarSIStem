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

    virtual glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) = 0;

    glm::mat4 getOnePosMatrix(glm::mat4 globalMVMatrix, uint index, float time) {
        assert(index <= m_satelites.size());
        if(index == 0){
            return getPosMatrix(globalMVMatrix, time);
        }
        return m_satelites[index - 1]->getPosMatrix(globalMVMatrix, time);
    }
};


struct PlanetObjects : public AStellarObject {
    float m_coef_diametre;
    float m_dist_sol;
    float m_orbitalPeriod; // en jours
    float m_dayLength; // en jours
    float m_orbitalInclinaison;
    glm::vec3 sattelites_initial_position; 

    PlanetObjects(Program& program, 
        std::vector<const GLchar*> textures_uniform_locations, 
        std::vector<GLuint> texturesIds,
        float coef_diametre,
        float dist_sol,
        float orbitalPeriod, 
        float dayLength, 
        float orbitalInclinaison
        ): AStellarObject {program, textures_uniform_locations, texturesIds}, 
        m_coef_diametre{coef_diametre}, m_dist_sol{dist_sol}, m_orbitalPeriod{orbitalPeriod},
        m_dayLength{dayLength}, m_orbitalInclinaison{orbitalInclinaison}
    {
        sattelites_initial_position = glm::vec3(m_dist_sol, 0, 0);
    }

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
        glm::mat4 planetMVMatrix = glm::rotate(globalMVMatrix, glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        planetMVMatrix = glm::rotate(planetMVMatrix, time / m_orbitalPeriod, glm::vec3(0, 1, 0)); // Translation * Rotation * Rotation
        planetMVMatrix = glm::translate(planetMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = planetMVMatrix;
        planetMVMatrix = glm::scale(planetMVMatrix, glm::vec3(m_coef_diametre, m_coef_diametre, m_coef_diametre)); // Translation * Rotation * Translation * Scale
        planetMVMatrix = glm::rotate(planetMVMatrix, time / m_dayLength, glm::vec3(0, 1, 0)); // Translation * Rotation

        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(planetMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(planetMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(ProjMatrix * planetMVMatrix));

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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 planetMVMatrix = glm::rotate(planetMVMatrix, glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0));
        planetMVMatrix =  glm::rotate(globalMVMatrix, glm::radians(180.f) + (time / m_orbitalPeriod), glm::vec3(0, 1, 0));
        planetMVMatrix = glm::translate(planetMVMatrix, sattelites_initial_position);
        planetMVMatrix = glm::rotate(planetMVMatrix, glm::radians(180.f) - (time / m_orbitalPeriod), glm::vec3(0, 1, 0));
        planetMVMatrix = glm::rotate(planetMVMatrix, -glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0));
        return planetMVMatrix;
    }
};
