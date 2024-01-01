//
// Created by Antonin Jean on 29/12/2023.
//

#pragma once

#include "../includes.hpp"

struct CallistoProgram : public PlanetObjects {
    CallistoProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_CALISTO,
                COEF_DISTANCE_CALISTO + COEF_DIAMETRE_JUPITER + COEF_DIAMETRE_CALISTO,
                16.689017f,
                16.689017f,
                0.19f
            }
    {}
};

struct GanymedeProgram : public PlanetObjects {
    GanymedeProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_GANYMEDE,
                COEF_DISTANCE_GANYMEDE + COEF_DIAMETRE_JUPITER + COEF_DIAMETRE_GANYMEDE,
                7.2f,
                7.2f,
                0.18f
            }
    {}
};

struct EuropaProgram : public PlanetObjects {
    EuropaProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_EUROPA,
                COEF_DISTANCE_EUROPA + COEF_DIAMETRE_JUPITER + COEF_DIAMETRE_EUROPA,
                3.6f,
                3.6f,
                0.47f
            }
    {}
};


struct IOProgram : public PlanetObjects {
    IOProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_IO,
                COEF_DISTANCE_IO + COEF_DIAMETRE_JUPITER + COEF_DIAMETRE_IO,
                1.8,
                1.8,
                0.04
            }
    {}
};
