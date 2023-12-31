//
// Created by Antonin Jean on 29/12/2023.
//


#pragma once
#include "../includes.hpp"


struct MoonProgram : public PlanetObjects {
    MoonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
        PlanetObjects {program, textures_uniform_locations, texturesIds,
            COEF_DIAMETRE_MOON,
            COEF_DISTANCE_MOON + COEF_DIAMETRE_TERRE + COEF_DIAMETRE_MOON,
            27.3f,
            708.7f / 24.f,
            5.1f
        }
    {}
};
