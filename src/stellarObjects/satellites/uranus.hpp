//
// Created by Antonin Jean on 29/12/2023.
//
#pragma once

#include "../includes.hpp"

struct ArielProgram : public PlanetObjects {
    ArielProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_ARIEL,
            COEF_DISTANCE_PERIHELION_ARIEL + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_ARIEL,
            COEF_DISTANCE_APHELION_ARIEL + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_ARIEL,
            2.520379f,
            2.520379f,
            0.04f
        }
    {}
};

struct UmbrielProgram : public PlanetObjects {
    UmbrielProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_UMBRIEL,
            COEF_DISTANCE_PERIHELION_UMBRIEL + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_UMBRIEL,
            COEF_DISTANCE_APHELION_UMBRIEL + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_UMBRIEL,
            4.144176f,
            4.144176f,
            0.13f
        }
    {}
};

struct TitaniaProgram : public PlanetObjects {
    TitaniaProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_TITANIA,
            COEF_DISTANCE_PERIHELION_TITANIA + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_TITANIA,
            COEF_DISTANCE_APHELION_TITANIA + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_TITANIA,
            8.705867f,
            8.705867f,
            0.08f
        }
    {}
};

struct OberonProgram : public PlanetObjects {
    OberonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
            PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_OBERON,
            COEF_DISTANCE_PERIHELION_OBERON + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_OBERON,
            COEF_DISTANCE_APHELION_OBERON + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_OBERON,
            13.463234f,
            13.463234f,
            0.07f
        }
    {}
};

struct MirandaProgram : public PlanetObjects {
    MirandaProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, RingsObject ring):
        PlanetObjects {program, textures_uniform_locations, texturesIds, ring,
            COEF_DIAMETRE_MIRANDA,
            COEF_DISTANCE_PERIHELION_MIRANDA + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_MIRANDA,
            COEF_DISTANCE_APHELION_MIRANDA + COEF_DIAMETRE_URANUS + COEF_DIAMETRE_MIRANDA,
            1.413479f,
            1.413479f,
            4.34f
        }
    {}
};
