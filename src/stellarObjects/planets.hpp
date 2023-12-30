//
// Created by Antonin Jean on 29/12/2023.
//
#pragma once

#include "includes.hpp"


struct MercureProgram : public AStellarObject {
    const float coef_diametre = 0.02439;
    const float dist_sol = 2.06;
    const float orbitalPeriod = 88.0f; // en jours
    const float dayLength = 4222.6f / 24.f; // en jours
    const float orbitalInclinaison = 7.f;
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

        glm::mat4 mercureMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        mercureMVMatrix = glm::rotate(mercureMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation * Rotation
        mercureMVMatrix = glm::translate(mercureMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = mercureMVMatrix;
        mercureMVMatrix = glm::scale(mercureMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        mercureMVMatrix = glm::rotate(mercureMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation

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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
};


struct VenusProgram : public AStellarObject {
    const float coef_diametre = 0.0605;
    const float dist_sol = 3.86;
    const float orbitalPeriod = 224.7f; // en jours
    const float dayLength = 2802.f / 24.f; // en jours
    const float orbitalInclinaison = 7.f;
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

        glm::mat4 venusMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        venusMVMatrix = glm::rotate(venusMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        venusMVMatrix = glm::translate(venusMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = venusMVMatrix;
        venusMVMatrix = glm::scale(venusMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        venusMVMatrix = glm::rotate(venusMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation
        
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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
};


struct EarthProgram : public AStellarObject {
    const float coef_diametre = 0.06376;
    const float dist_sol = 5.1675;
    const float orbitalPeriod = 365.2f; // en jour
    const float dayLength = 24.f / 24.f; // en jour
    const float orbitalInclinaison = 0.f;
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

        glm::mat4 earthMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        earthMVMatrix = glm::rotate(earthMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        earthMVMatrix = glm::translate(earthMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = earthMVMatrix;
        earthMVMatrix = glm::scale(earthMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        earthMVMatrix = glm::rotate(earthMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation

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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
};


struct MarsProgram : public AStellarObject {
    const float coef_diametre = 0.0339;
    const float dist_sol = 8.14;
    const float orbitalPeriod = 687.f;
    const float dayLength = 24.7f / 24.f; // en jours
    const float orbitalInclinaison = 1.8f;
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

        glm::mat4 marsMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        marsMVMatrix = glm::rotate(marsMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        marsMVMatrix = glm::translate(marsMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = marsMVMatrix;
        marsMVMatrix = glm::scale(marsMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        marsMVMatrix = glm::rotate(marsMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation
        
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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
};


struct JupiterProgram : public AStellarObject {
    const float coef_diametre = 0.7149;
    const float dist_sol = 27.80;
    const float orbitalPeriod = 4331.f;
    const float dayLength = 9.9f / 24.f;
    const float orbitalInclinaison = 1.3f;
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

        glm::mat4 jupiterMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        jupiterMVMatrix = glm::rotate(jupiterMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        jupiterMVMatrix = glm::translate(jupiterMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = jupiterMVMatrix;
        jupiterMVMatrix = glm::scale(jupiterMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        jupiterMVMatrix = glm::rotate(jupiterMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation
        
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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, time, sattelites_rotation_axis);
        return glm::translate(MVMatrix, sattelites_initial_position);
    }
};

struct SaturneProgram : public AStellarObject {
    const float coef_diametre = 0.6026;
    const float dist_sol = 51.14;
    const float orbitalPeriod = 10747.f;
    const float dayLength = 10.7f / 24.f;
    const float orbitalInclinaison = 2.5f;
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

        glm::mat4 saturneMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        saturneMVMatrix = glm::rotate(saturneMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        saturneMVMatrix = glm::translate(saturneMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = saturneMVMatrix;
        saturneMVMatrix = glm::scale(saturneMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        saturneMVMatrix = glm::rotate(saturneMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation
        
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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, time, sattelites_rotation_axis);
        return glm::translate(MVMatrix, sattelites_initial_position);
    }
};

struct UranusProgram : public AStellarObject {
    const float coef_diametre = 0.2555;
    const float dist_sol = 102.39;
    const float orbitalPeriod = 30589.f;
    const float dayLength = 17.2f / 24.f;
    const float orbitalInclinaison = .8f;
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

        glm::mat4 uranusMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        uranusMVMatrix = glm::rotate(uranusMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        uranusMVMatrix = glm::translate(uranusMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = uranusMVMatrix;
        uranusMVMatrix = glm::scale(uranusMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        uranusMVMatrix = glm::rotate(uranusMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation
        
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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
};

struct NeptuneProgram : public AStellarObject {
    const float coef_diametre = 0.2476;
    const float dist_sol = 161.25;
    const float orbitalPeriod = 59800.f;
    const float dayLength = 16.1f / 24.f;
    const float orbitalInclinaison = 1.8f;
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

        glm::mat4 neptuneMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        neptuneMVMatrix = glm::rotate(neptuneMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        neptuneMVMatrix = glm::translate(neptuneMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = neptuneMVMatrix;
        neptuneMVMatrix = glm::scale(neptuneMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        neptuneMVMatrix = glm::rotate(neptuneMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation
        
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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
};

struct PlutonProgram : public AStellarObject {
    const float coef_diametre = 0.0118;
    const float dist_sol = 210.94;
    const float orbitalPeriod = 90560.f;
    const float dayLength = 153.3f / 24.f;
    const float orbitalInclinaison = 17.2f;
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

        glm::mat4 plutonMVMatrix = glm::rotate(globalMVMatrix, glm::radians(orbitalInclinaison), glm::vec3(1, 0, 0)); // Translation * Rotation
        plutonMVMatrix = glm::rotate(plutonMVMatrix, time / orbitalPeriod, sattelites_rotation_axis); // Translation * Rotation
        plutonMVMatrix = glm::translate(plutonMVMatrix, sattelites_initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = plutonMVMatrix;
        plutonMVMatrix = glm::scale(plutonMVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        plutonMVMatrix = glm::rotate(plutonMVMatrix, time / dayLength, sattelites_rotation_axis); // Translation * Rotation
        
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

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
};