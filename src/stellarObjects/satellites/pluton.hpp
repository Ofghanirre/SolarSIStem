//
// Created by Antonin Jean on 29/12/2023.
//


#pragma once
#include "../includes.hpp"


struct CharonProgram : public PlanetObjects {
    CharonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
        PlanetObjects {program, textures_uniform_locations, texturesIds,
            COEF_DIAMETRE_CHARON,
            COEF_DISTANCE_CHARON + COEF_DIAMETRE_PLUTON,
            27.3f,
            708.7f / 24.f,
            0.00005
        }
    {}
};
