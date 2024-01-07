#pragma once

#include "libs.hpp"
#include "utils.hpp"
#include "rings.hpp"
#include "context.hpp"
#include <glimac/Sphere.hpp>
#include <iostream>
#include <cassert>

using namespace glimac;

float carre(float a) {
    return a * a;
}

float cosCarre(float a) {
    return (1 + glm::cos(2 * a)) / 2;
}

float sinCarre(float a) {
    return (1 - glm::cos(2 * a)) / 2;
}

/**
    @brief A StellarObject is an Object that can be represented in 3D, that possess 
    a set of Satelittes (other StellarObjects)

    It serves as a superclass drawing the standard behaviour for planets, stars, etc.
*/
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
    GLuint m_uLightSourceLocation;
    GLuint m_uViewPosition;
    GLuint m_isLightOnLocation;
    AStellarObject(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds) : 
        m_Program{program}
    {
        assert(textures_uniform_locations.size() < 3);
        assert(textures_uniform_locations.size() == texturesIds.size());
        m_Program.use();
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        for (uint i = 0; i < texturesIds.size(); i++){
            m_texturesIds.emplace_back(texturesIds[i]);
            m_texturesNames.emplace_back(textures_uniform_locations[i]);
            m_textures.emplace_back(checkValid(glGetUniformLocation(m_Program.getGLId(), textures_uniform_locations[i]), textures_uniform_locations[i]));
        }
        m_uLightSourceLocation = glGetUniformLocation(m_Program.getGLId(), "uLightSource");
        m_uViewPosition = glGetUniformLocation(m_Program.getGLId(), "uViewPosition");
        m_isLightOnLocation = glGetUniformLocation(m_Program.getGLId(), "isLightOn");
    }

    void update() {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        for (auto textureName: m_texturesNames){
            m_textures.emplace_back(checkValid(glGetUniformLocation(m_Program.getGLId(), textureName), textureName));
        }
        m_uLightSourceLocation = glGetUniformLocation(m_Program.getGLId(), "uLightSource");
        m_uViewPosition = glGetUniformLocation(m_Program.getGLId(), "uViewPosition");
    }
    
    
    void addSatelite(AStellarObject* satelite) {
        m_satelites.emplace_back(satelite);
    }

    /**
        @brief Function to display a StellarObject and its satelittes into a scene
    */
    void drawAll(glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix,
        float time,
        bool traj,
        GeometricalContext context,
        glm::mat4 lightSourceMatrix,
        int isLightOn
    ) {
        glm::mat4 matrixPos = draw(globalMVMatrix, viewMatrix, time, traj, context, lightSourceMatrix, isLightOn);
        for(auto satelite : m_satelites){
            satelite->drawAll(matrixPos, viewMatrix, time, traj, context, lightSourceMatrix, isLightOn);
        }
    }

    void use() {
        m_Program.use();
    }

    /**
        @brief Function to represent the object into a scene
        This function is to be called by the drawAll method
    */
    virtual glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix,
        float time, 
        bool traj,
        GeometricalContext context,
        glm::mat4 lightSourceMatrix,
        int isLightOn
    ) = 0;

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
    float m_orbitalPeriod; // en jours
    float m_dayLength; // en jours
    float m_orbitalInclinaison;
    float m_aphelion;
    float m_perihelion;
    float m_nominateur;
    RingsObject m_rings;

    PlanetObjects(Program& program, 
        std::vector<const GLchar*> textures_uniform_locations, 
        std::vector<GLuint> texturesIds,
        RingsObject ring,
        float coef_diametre,
        //float dist_sol,
        float perihelion, 
        float aphelion,
        float orbitalPeriod, 
        float dayLength, 
        float orbitalInclinaison
        ): AStellarObject {program, textures_uniform_locations, texturesIds}, 
        m_rings{ring}, m_coef_diametre{coef_diametre}, 
        m_aphelion{aphelion}, m_perihelion{perihelion},
        m_orbitalPeriod{orbitalPeriod}, m_dayLength{dayLength},
        m_orbitalInclinaison{orbitalInclinaison}
    {
        m_nominateur = m_aphelion * m_aphelion * m_perihelion * m_perihelion;
    }

    glm::vec3 getDistForAngleElipse(float angle) {
        float denominateur = carre(m_aphelion) * sinCarre(angle) + carre(m_perihelion) * cosCarre(angle);
        return glm::vec3(glm::sqrt(m_nominateur / denominateur), 0, 0);
    }

    glm::mat4 draw(
        glm::mat4 globalMVMatrix,
        glm::mat4 viewMatrix,
        float time,
        bool traj,
        GeometricalContext context,
        glm::mat4 lightSourceMatrix ,
        int isLightOn
    ) override
    {
        glm::mat4 planetMVMatrix = glm::rotate(globalMVMatrix, glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        if (traj) {
            m_rings.draw(planetMVMatrix, m_aphelion, m_perihelion, context.ctxtCircle);
        }
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }
        planetMVMatrix = glm::rotate(planetMVMatrix, time / m_orbitalPeriod, glm::vec3(0, 1, 0)); // Translation * Rotation * Rotation
        planetMVMatrix = glm::translate(planetMVMatrix, getDistForAngleElipse(time / m_orbitalPeriod)); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = planetMVMatrix;
        planetMVMatrix = glm::scale(planetMVMatrix, glm::vec3(m_coef_diametre, m_coef_diametre, m_coef_diametre)); // Translation * Rotation * Translation * Scale
        planetMVMatrix = glm::rotate(planetMVMatrix, time / m_dayLength, glm::vec3(0, 1, 0)); // Translation * Rotation

        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(planetMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(planetMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(context.ctxtSphere.ProjMatrix * planetMVMatrix));

        // LIGHT INFO
        // Position initiale du soleil (au centre du modèle)
        glm::vec3 sunPositionModelSpace = glm::vec3(0.0f, 0.0f, 0.0f);
        // Calcul de la position du soleil dans le modèle
        glm::vec3 sunPositionWorldSpace = glm::vec3(lightSourceMatrix * glm::vec4(sunPositionModelSpace, 1.0f));

        // Calcul de la position du soleil dans l'espace de vue
        glm::vec3 sunPositionViewSpace = glm::vec3(viewMatrix * glm::vec4(sunPositionWorldSpace, 1.0f));
        glUniform3f(m_uLightSourceLocation, sunPositionViewSpace.x, sunPositionViewSpace.y, sunPositionViewSpace.z);
        glUniform3f(m_uViewPosition, viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]);
        glUniform1i(m_isLightOnLocation, isLightOn);
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(context.ctxtSphere.vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, context.ctxtSphere.m_shape->getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
        return MVMatrixPos;
    }

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 planetMVMatrix = glm::rotate(planetMVMatrix, glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0));
        planetMVMatrix =  glm::rotate(globalMVMatrix, glm::radians(180.f) + (time / m_orbitalPeriod), glm::vec3(0, 1, 0));
        planetMVMatrix = glm::translate(planetMVMatrix, getDistForAngleElipse(time / m_orbitalPeriod));
        planetMVMatrix = glm::rotate(planetMVMatrix, glm::radians(180.f) - (time / m_orbitalPeriod), glm::vec3(0, 1, 0));
        planetMVMatrix = glm::rotate(planetMVMatrix, -glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0));
        return planetMVMatrix;
    }
};


struct RingedPlanetObjects : public AStellarObject {
    float m_coef_diametre;
    float m_perihelion; 
    float m_aphelion;
    float m_orbitalPeriod; // en jours
    float m_dayLength; // en jours
    float m_orbitalInclinaison;
    float m_nominateur;
    float m_ring_radius;
    RingsObject m_rings;
    FilledRingsObject m_filled_ring;
    GLuint m_texture_id;
    RingedPlanetObjects(Program& program,
                  std::vector<const GLchar*> textures_uniform_locations,
                  std::vector<GLuint> texturesIds,
                  RingsObject ring,
                  FilledRingsObject filled_ring,
                  GLuint ring_texture_id,
                  float coef_diametre,
                  float perihelion, 
                  float aphelion,
                  float orbitalPeriod,
                  float dayLength,
                  float orbitalInclinaison,
                  float ringRadius
    ): AStellarObject {program, textures_uniform_locations, texturesIds},
       m_rings{ring}, m_filled_ring{filled_ring}, m_texture_id{ring_texture_id}, m_coef_diametre{coef_diametre},
       m_aphelion{aphelion}, m_perihelion{perihelion},
       m_orbitalPeriod{orbitalPeriod}, m_dayLength{dayLength},
       m_orbitalInclinaison{orbitalInclinaison}, m_ring_radius{ringRadius}
    {
        m_nominateur = m_aphelion * m_aphelion * m_perihelion * m_perihelion;
    }

    glm::vec3 getDistForAngleElipse(float angle) {
        float denominateur = carre(m_aphelion) * sinCarre(angle) + carre(m_perihelion) * cosCarre(angle);
        return glm::vec3(glm::sqrt(m_nominateur / denominateur), 0, 0);
    }

    glm::mat4 draw(
            glm::mat4 globalMVMatrix,
            glm::mat4 viewMatrix,
            float time,
            bool traj,
            GeometricalContext context,
            glm::mat4 lightSourceMatrix,
            int isLightOn 
    ) override
    {
        glm::mat4 planetMVMatrix = glm::rotate(globalMVMatrix, glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        if (traj) {
            m_rings.draw(planetMVMatrix, m_aphelion, m_perihelion, context.ctxtCircle);
        }
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }
        planetMVMatrix = glm::rotate(planetMVMatrix, time / m_orbitalPeriod, glm::vec3(0, 1, 0)); // Translation * Rotation * Rotation
        planetMVMatrix = glm::translate(planetMVMatrix, getDistForAngleElipse(time / m_orbitalPeriod)); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = planetMVMatrix;
        planetMVMatrix = glm::scale(planetMVMatrix, glm::vec3(m_coef_diametre, m_coef_diametre, m_coef_diametre)); // Translation * Rotation * Translation * Scale
        planetMVMatrix = glm::rotate(planetMVMatrix, time / m_dayLength, glm::vec3(0, 1, 0)); // Translation * Rotation

        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(planetMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(planetMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(context.ctxtSphere.ProjMatrix * planetMVMatrix));
        // LIGHT INFO
        // Position initiale du soleil (au centre du modèle)
        glm::vec3 sunPositionModelSpace = glm::vec3(0.0f, 0.0f, 0.0f);
                // Calcul de la position du soleil dans le modèle
        glm::vec3 sunPositionWorldSpace = glm::vec3(lightSourceMatrix * glm::vec4(sunPositionModelSpace, 1.0f));
        // Calcul de la position du soleil dans l'espace de vue
        glm::vec3 sunPositionViewSpace = glm::vec3(viewMatrix * glm::vec4(sunPositionWorldSpace, 1.0f));
        glUniform3f(m_uLightSourceLocation, sunPositionViewSpace.x, sunPositionViewSpace.y, sunPositionViewSpace.z);
        glUniform3f(m_uViewPosition, viewMatrix[2][0], viewMatrix[2][1], viewMatrix[2][2]);
        glUniform1i(m_isLightOnLocation, isLightOn);

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(context.ctxtSphere.vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, context.ctxtSphere.m_shape->getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao

        m_filled_ring.draw(planetMVMatrix, m_ring_radius, context.ctxtRing, m_texture_id, sunPositionViewSpace, isLightOn);
        return MVMatrixPos;
    }

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 planetMVMatrix = glm::rotate(planetMVMatrix, glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0));
        planetMVMatrix =  glm::rotate(globalMVMatrix, glm::radians(180.f) + (time / m_orbitalPeriod), glm::vec3(0, 1, 0));
        planetMVMatrix = glm::translate(planetMVMatrix, getDistForAngleElipse(time / m_orbitalPeriod));
        planetMVMatrix = glm::rotate(planetMVMatrix, glm::radians(180.f) - (time / m_orbitalPeriod), glm::vec3(0, 1, 0));
        planetMVMatrix = glm::rotate(planetMVMatrix, -glm::radians(m_orbitalInclinaison), glm::vec3(1, 0, 0));
        return planetMVMatrix;
    }
};