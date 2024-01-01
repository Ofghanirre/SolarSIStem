#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
class Circle {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat radius, GLsizei discLat);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Circle(GLfloat radius, GLsizei discLat):
        m_nVertexCount(0) {
        build(radius, discLat); // Construction (voir le .cpp)
    }

    // Renvoit le pointeur vers les données
    const ShapeVertex* getDataPointer() const {
        return &m_Vertices[0];
    }
    
    // Renvoit le nombre de vertex
    GLsizei getVertexCount() const {
        return m_nVertexCount;
    }

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
};
    
}