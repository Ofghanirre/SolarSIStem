//
// Created by Antonin Jean on 01/01/2024.
//

#include <vector>

#include "common.hpp"
#include "Shape.hpp"

namespace glimac {

    class Ellipse : public Shape {
        // Alloue et construit les données (implantation dans le .cpp)
        void build(GLfloat width, GLfloat height, GLsizei amountOfPoints);

    public:
        // Constructeur: alloue le tableau de données et construit les attributs des vertex
        Ellipse(GLfloat width, GLsizei height, GLsizei amountOfPoints)
        {
            build(width, height, amountOfPoints); // Construction (voir le .cpp)
        }
    };
}
