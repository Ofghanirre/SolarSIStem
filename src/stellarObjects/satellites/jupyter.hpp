//
// Created by Antonin Jean on 29/12/2023.
//

#pragma once

#include "../includes.hpp"

struct CallistoProgram : public PlanetObjects {
    float coef_diametre = 0.0241;
    float dist_jupyter = 0.06721428571 + COEF_DIAMETRE_JUPITER;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_jupyter, 0, 0); //glm::sphericalRand(2.f);

    CallistoProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
            PlanetObjects {program, textures_uniform_locations, texturesIds,
                COEF_DIAMETRE_CALISTO,
                COEF_DISTANCE_CALISTO + COEF_DIAMETRE_JUPITER,
                16.689017f,
                16.689017f,
                0.19f
            }
    {}
};

struct GanymedeProgram : public PlanetObjects {
    float coef_diametre = 0.026312;
    float dist_jupyter = 0.03822857143 + COEF_DIAMETRE_JUPITER;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_jupyter, 0, 0); //glm::sphericalRand(2.f);

    GanymedeProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
            PlanetObjects {program, textures_uniform_locations, texturesIds,
                COEF_DIAMETRE_GANYMEDE,
                COEF_DISTANCE_GANYMEDE + COEF_DIAMETRE_JUPITER,
                7.2f,
                7.2f,
                0.18f
            }
    {}
};

struct EuropaProgram : public PlanetObjects {
    float coef_diametre = 0.015608;
    float dist_jupyter = 0.02396785714 + COEF_DIAMETRE_JUPITER;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_jupyter, 0, 0); //glm::sphericalRand(2.f);

    EuropaProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
            PlanetObjects {program, textures_uniform_locations, texturesIds,
                COEF_DIAMETRE_EUROPA,
                COEF_DISTANCE_EUROPA + COEF_DIAMETRE_JUPITER,
                3.6f,
                3.6f,
                0.47f
            }
    {}
};


struct IOProgram : public PlanetObjects {
    float coef_diametre = 0.018215;
    float dist_jupyter = 0.01506428571 + COEF_DIAMETRE_JUPITER;
    glm::vec3 sattelites_rotation_axis = glm::vec3(0, 1, 0); //glm::sphericalRand(1.f);
    glm::vec3 sattelites_initial_position = glm::vec3(dist_jupyter, 0, 0); //glm::sphericalRand(2.f);

    IOProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
            PlanetObjects {program, textures_uniform_locations, texturesIds,
                COEF_DIAMETRE_IO,
                COEF_DISTANCE_IO + COEF_DIAMETRE_JUPITER,
                1.8,
                1.8,
                0.04
            }
    {}
};
