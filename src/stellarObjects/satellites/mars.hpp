//
// Created by Antonin Jean on 29/12/2023.
//

#pragma once

struct PhobosProgram : public PlanetObjects {
    PhobosProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
            PlanetObjects {program, textures_uniform_locations, texturesIds, 
                COEF_DIAMETRE_PHOBOS,
                COEF_DISTANCE_PHOBOS + COEF_DIAMETRE_MARS + COEF_DIAMETRE_PHOBOS,
                0.31891f,
                0.31891f,
                1.08f
            }
    {}
};

struct DeimosProgram : public PlanetObjects {
    DeimosProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds):
            PlanetObjects {program, textures_uniform_locations, texturesIds, 
                COEF_DIAMETRE_DEIMOS,
                COEF_DISTANCE_DEIMOS + COEF_DIAMETRE_MARS + COEF_DIAMETRE_DEIMOS,
                1.26244f,
                1.26244f,
                1.79f
            }
    {}
};
