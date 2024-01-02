#pragma once

#include <vector>

#include "common.hpp"
#include "Shape.hpp"

namespace glimac {

class Ring : public Shape {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat maxradius, GLfloat minradius, GLsizei discLat);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Ring(GLfloat maxradius, GLfloat minradius, GLsizei discLat){
        build(maxradius, minradius, discLat); // Construction (voir le .cpp)
    }
};
    
}