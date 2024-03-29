//
// Created by Antonin Jean on 29/12/2023.
//


#pragma once
#include "../includes.hpp"


struct CharonProgram : public PlanetObjects {
    CharonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_CHARON,
            COEF_DISTANCE_PERIHELION_CHARON + COEF_DIAMETRE_PLUTON + COEF_DIAMETRE_CHARON,
            COEF_DISTANCE_APHELION_CHARON + COEF_DIAMETRE_PLUTON + COEF_DIAMETRE_CHARON,
            6.3872f,
            6.3872f,
            0.00005
        }
    {}
};
