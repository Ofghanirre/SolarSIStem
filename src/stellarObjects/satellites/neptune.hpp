//
// Created by Antonin Jean on 29/12/2023.
//


#pragma once
#include "../includes.hpp"


struct TritonProgram : public PlanetObjects {
    TritonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
        PlanetObjects {program, textures_uniform_locations, texturesIds, 
            COEF_DIAMETRE_TRITON,
            COEF_DISTANCE_TRITON + COEF_DIAMETRE_NEPTUNE,
            5.876f,
            5.876f,
            157.345
        }
    {}
};

struct NereideProgram : public PlanetObjects {
    const float coef_diametre = 0.0017;
    const float dist_earth = 0.1969071429 + COEF_DIAMETRE_NEPTUNE;
    const float orbitalPeriod = 27.3f; // en jours
    const float dayLength = 708.7f / 24.f; // en jours
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_earth, 0, 0); //glm::sphericalRand(2.f);

    NereideProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
        PlanetObjects {program, textures_uniform_locations, texturesIds, 
            COEF_DIAMETRE_NERIEDE,
            COEF_DISTANCE_NERIEDE + COEF_DIAMETRE_NEPTUNE,
            360.13619, 
            DEFAULT_VALUE,
            7.23
        }
    {}
};
