//
// Created by Antonin Jean on 29/12/2023.
//

#pragma once

#include "../includes.hpp"


struct MimasProgram : public PlanetObjects {
    MimasProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_MIMAS,
                COEF_DISTANCE_PERIHELION_MIMAS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_MIMAS,
                COEF_DISTANCE_APHELION_MIMAS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_MIMAS,
                0.9424218f,
                0.9424218f,
                1.53f
            }
    {}
};


struct EnceladusProgram : public PlanetObjects {
    EnceladusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_ENCELADUS,
                COEF_DISTANCE_PERIHELION_ENCELADUS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_ENCELADUS,
                COEF_DISTANCE_APHELION_ENCELADUS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_ENCELADUS,
                1.370218f,
                1.370218f,
                0.f
            }
    {}
};

struct TethysProgram : public PlanetObjects {
    TethysProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_TETHYS,
                COEF_DISTANCE_PERIHELION_TETHYS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_TETHYS,
                COEF_DISTANCE_APHELION_TETHYS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_TETHYS,
                1.887802f,
                1.887802f,
                1.86f
            }
    {}
};

struct DioneProgram : public PlanetObjects {
    DioneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_DIONE,
                COEF_DISTANCE_PERIHELION_DIONE + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_DIONE,
                COEF_DISTANCE_APHELION_DIONE + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_DIONE,
                2.736915f,
                2.736915f,
                0.02f
            }
    {}
};

struct RheaProgram : public PlanetObjects {
    RheaProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_RHEA,
                COEF_DISTANCE_PERIHELION_RHEA + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_RHEA,
                COEF_DISTANCE_APHELION_RHEA + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_RHEA,
                4.517500f,
                4.517500f,
                0.35f
            }
    {}
};

struct TitanProgram : public PlanetObjects {
    TitanProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_TITAN,
                COEF_DISTANCE_PERIHELION_TITAN + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_TITAN,
                COEF_DISTANCE_APHELION_TITAN + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_TITAN,
                15.945421f,
                15.945421f,
                .33f
            }
    {}
};

struct HyperionProgram : public PlanetObjects {
    HyperionProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_HYPERION,
                COEF_DISTANCE_PERIHELION_HYPERION + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_HYPERION,
                COEF_DISTANCE_APHELION_HYPERION + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_HYPERION,
                21.276609f,
                21.276609f,
                .43f
            }
    {}
};

struct IapetusProgram : public PlanetObjects {
    IapetusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
                COEF_DIAMETRE_IAPETUS,
                COEF_DISTANCE_PERIHELION_IAPETUS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_IAPETUS,
                COEF_DISTANCE_APHELION_IAPETUS + COEF_DIAMETRE_SATURNE + COEF_DIAMETRE_IAPETUS,
                79.330183f,
                79.330183f,
                14.72f
            }
    {}
};