//
// Created by Antonin Jean on 29/12/2023.
//
#pragma once

#include "includes.hpp"


struct MercureProgram : public PlanetObjects {
    MercureProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_MERCURE, //coef_diametre
                COEF_DISTANCE_MERCURE + COEF_DIAMETRE_SUN, //dist_sol
                88.0f, //orbitalPeriod
                4222.6f / 24.f, //dayLength
                7.f //orbitalInclinaison
            }
    {}
};


struct VenusProgram : public PlanetObjects {
    VenusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_VENUS, //coef_diametre
                COEF_DISTANCE_VENUS + COEF_DIAMETRE_SUN, //dist_sol
                224.7f, //orbitalPeriod
                2802.f / 24.f, //dayLength
                3.4f //orbitalInclinaison
            }
    {}
};


struct EarthProgram : public PlanetObjects {
    EarthProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_TERRE, //coef_diametre
                COEF_DISTANCE_TERRE + COEF_DIAMETRE_SUN, //dist_sol
                365.2f, //orbitalPeriod
                24.f / 24.f, //dayLength
                0.f //orbitalInclinaison
            }
    {}
};


struct MarsProgram : public PlanetObjects {
    MarsProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_MARS, //coef_diametre
                COEF_DISTANCE_MARS + COEF_DIAMETRE_SUN, //dist_sol
                687.f, //orbitalPeriod
                24.7f / 24.f, //dayLength
                1.8f //orbitalInclinaison
            }
    {}
};


struct JupiterProgram : public PlanetObjects {
    JupiterProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_JUPITER, //coef_diametre
                COEF_DISTANCE_JUPITER + COEF_DIAMETRE_SUN, //dist_sol
                4331.f, //orbitalPeriod
                9.9f / 24.f, //dayLength
                1.3f //orbitalInclinaison
            }
    {}
};

struct SaturneProgram : public RingedPlanetObjects {
    SaturneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring, FilledRingsObject filled_ring, GLuint textureId):
            RingedPlanetObjects {program, textures_uniform_locations, texturesIds, ring, filled_ring, textureId,
                COEF_DIAMETRE_SATURNE, //coef_diametre
                COEF_DISTANCE_SATURNE + COEF_DIAMETRE_SUN, //dist_sol
                10747.f, //orbitalPeriod
                10.7f / 24.f, //dayLength
                2.5f, //orbitalInclinaison
                DISTANCE_RADIUS_SATURNE_RING // ring diametre
            }
    {}
};

struct UranusProgram : public PlanetObjects {
    UranusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_URANUS, //coef_diametre
                COEF_DISTANCE_URANUS + COEF_DIAMETRE_SUN, //dist_sol
                30589.f, //orbitalPeriod
                17.2f / 24.f, //dayLength
                .8f //orbitalInclinaison
            }
    {}
};

struct NeptuneProgram : public PlanetObjects {
    NeptuneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_NEPTUNE, //coef_diametre
                COEF_DISTANCE_NEPTUNE + COEF_DIAMETRE_SUN, //dist_sol
                59800.f, //orbitalPeriod
                16.1f / 24.f, //dayLength
                1.8f //orbitalInclinaison
            }
    {}
};

struct PlutonProgram : public PlanetObjects {
    PlutonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_PLUTON, //coef_diametre
                COEF_DISTANCE_PLUTON + COEF_DIAMETRE_SUN, //dist_sol
                90560.f, //orbitalPeriod
                153.3f / 24.f, //dayLength
                17.2f //orbitalInclinaison
            }
    {}
};
