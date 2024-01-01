//
// Created by Antonin Jean on 01/01/2024.
//

#include <vector>

#include "common.hpp"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
    class Ellipse : public Shape {
        // Alloue et construit les données (implantation dans le .cpp)
        void build(GLfloat width, GLfloat height, GLsizei amountOfPoints);

    public:
        // Constructeur: alloue le tableau de données et construit les attributs des vertex
        Ellipse(GLfloat radius, GLsizei discLat, GLsizei discLong):
                m_nVertexCount(0) {
            build(radius, discLat, discLong); // Construction (voir le .cpp)
        }
    private:
        std::vector<ShapeVertex> m_Vertices;
        GLsizei m_nVertexCount; // Nombre de sommets
    };

}
