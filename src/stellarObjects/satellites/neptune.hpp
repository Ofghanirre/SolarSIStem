//
// Created by Antonin Jean on 29/12/2023.
//


#pragma once
#include "../includes.hpp"


struct TritonProgram : public PlanetObjects {
    TritonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_TRITON,
            COEF_DISTANCE_TRITON + COEF_DIAMETRE_NEPTUNE + COEF_DIAMETRE_TRITON,
            5.876f,
            5.876f,
            157.345
        }
    {}
};

struct NereideProgram : public PlanetObjects {
    NereideProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_NERIEDE,
            COEF_DISTANCE_NERIEDE + COEF_DIAMETRE_NEPTUNE + COEF_DIAMETRE_NERIEDE,
            360.13619, 
            DEFAULT_VALUE,
            7.23
        }
    {}
};
