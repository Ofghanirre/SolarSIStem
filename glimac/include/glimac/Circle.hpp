#pragma once

#include <vector>

#include "common.hpp"
#include "Shape.hpp"


namespace glimac {

    class Circle : public Shape {
        // Alloue et construit les données (implantation dans le .cpp)
        void build(GLfloat radius, GLsizei discLat);

    public:
        // Constructeur: alloue le tableau de données et construit les attributs des vertex
        Circle(GLfloat radius, GLsizei discLat) {
            build(radius, discLat); // Construction (voir le .cpp)
        }
    };
    
}