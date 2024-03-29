//
// Created by Antonin Jean on 29/12/2023.
//
#pragma once

#include "includes.hpp"

#define TIME_ROTATE_SUN 25

struct SunProgram : public AStellarObject{
    float coef_diametre = COEF_DIAMETRE_SUN;
    const float dist_sol = 0.0f;
    GLuint m_uTimeLocation;

    SunProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
            AStellarObject {program, textures_uniform_locations, texturesIds}
    {
        m_uTimeLocation = glGetUniformLocation(m_Program.getGLId(), "uTime");
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
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 MVMatrixPlanetView = globalMVMatrix * viewMatrix;
        glm::mat4 sunMVMatrix = glm::rotate(MVMatrixPlanetView, time / TIME_ROTATE_SUN, glm::vec3(0, 1, 0));
        sunMVMatrix = glm::scale(sunMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre));
        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(sunMVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(sunMVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(context.ctxtSphere.ProjMatrix * sunMVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }
        glUniform1i(m_isLightOnLocation, isLightOn);
        glUniform1f(m_uTimeLocation, time);

        glBindVertexArray(context.ctxtSphere.vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, context.ctxtSphere.m_shape->getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao

        return globalMVMatrix * viewMatrix;
    }

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        return globalMVMatrix;
    }
};
