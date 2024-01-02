//
// Created by Antonin Jean on 29/12/2023.
//
#pragma once

#include "includes.hpp"


struct MercureProgram : public PlanetObjects {
    MercureProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_MERCURE, //coef_diametre
                COEF_DISTANCE_PERIHELION_MERCURE + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_MERCURE + COEF_DIAMETRE_SUN,
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
                COEF_DISTANCE_PERIHELION_VENUS + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_VENUS + COEF_DIAMETRE_SUN,
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
                COEF_DISTANCE_PERIHELION_TERRE + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_TERRE + COEF_DIAMETRE_SUN,
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
                COEF_DISTANCE_PERIHELION_MARS + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_MARS + COEF_DIAMETRE_SUN,
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
                COEF_DISTANCE_PERIHELION_JUPITER + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_JUPITER + COEF_DIAMETRE_SUN,
                4331.f, //orbitalPeriod
                9.9f / 24.f, //dayLength
                1.3f //orbitalInclinaison
            }
    {}
};

struct SaturneProgram : public PlanetObjects {
    SaturneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_SATURNE, //coef_diametre
                COEF_DISTANCE_PERIHELION_SATURNE + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_SATURNE + COEF_DIAMETRE_SUN,
                10747.f, //orbitalPeriod
                10.7f / 24.f, //dayLength
                2.5f //orbitalInclinaison
            }
    {}
};

struct UranusProgram : public PlanetObjects {
    UranusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_URANUS, //coef_diametre
                COEF_DISTANCE_PERIHELION_URANUS + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_URANUS + COEF_DIAMETRE_SUN,
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
                COEF_DISTANCE_PERIHELION_NEPTUNE + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_NEPTUNE + COEF_DIAMETRE_SUN,
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
                COEF_DISTANCE_PERIHELION_PLUTON + COEF_DIAMETRE_SUN,
                COEF_DISTANCE_APHELION_PLUTON + COEF_DIAMETRE_SUN,
                90560.f, //orbitalPeriod
                153.3f / 24.f, //dayLength
                17.2f //orbitalInclinaison
            }
    {}
};
