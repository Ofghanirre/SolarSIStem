#pragma once

#include <vector>

#include "common.hpp"

namespace glimac {

class Ring : public Shape {
    // Alloue et construit les données (implantation dans le .cpp)
    void build(GLfloat maxradius, Glfloat minradius, GLsizei discLat);

public:
    // Constructeur: alloue le tableau de données et construit les attributs des vertex
    Ring(GLfloat maxradius, GLfloat minradius, GLsizei discLat):
        m_nVertexCount(0) {
        build(maxradius, minradius, discLat); // Construction (voir le .cpp)
    }

private:
    std::vector<ShapeVertex> m_Vertices;
    GLsizei m_nVertexCount; // Nombre de sommets
};
    
}