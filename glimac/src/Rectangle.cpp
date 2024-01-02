#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Rectangle.hpp"

namespace glimac {

void Rectangle::build(GLfloat width, GLfloat height, GLfloat depth) {
    std::vector<ShapeVertex> data;
    GLfloat mwidth = width/2;
    GLfloat mheight = height/2;
    GLfloat mdepth= depth/2;
    // Square construction
    data.push_back(ShapeVertex{glm::vec3{-mwidth, -mheight, -mdepth}});
    data.push_back(ShapeVertex{glm::vec3{mwidth, -mheight, -mdepth}});
    data.push_back(ShapeVertex{glm::vec3{mwidth, mheight, -mdepth}});
    data.push_back(ShapeVertex{glm::vec3{-mwidth, mheight, -mdepth}});
    data.push_back(ShapeVertex{glm::vec3{-mwidth, mheight, mdepth}});
    data.push_back(ShapeVertex{glm::vec3{mwidth, mheight, mdepth}});
    data.push_back(ShapeVertex{glm::vec3{mwidth, -mheight, mdepth}});
    data.push_back(ShapeVertex{glm::vec3{-mwidth, -mheight, mdepth}});
    m_nVertexCount = 6 * 2 * 3;

    // Face avant
    m_Vertices.push_back(data[0]);
    m_Vertices.push_back(data[1]);
    m_Vertices.push_back(data[2]);

    m_Vertices.push_back(data[0]);
    m_Vertices.push_back(data[2]);
    m_Vertices.push_back(data[3]);

    // Face arriere
    m_Vertices.push_back(data[4]);
    m_Vertices.push_back(data[5]);
    m_Vertices.push_back(data[6]);

    m_Vertices.push_back(data[4]);
    m_Vertices.push_back(data[6]);
    m_Vertices.push_back(data[7]);

    // Face haute
    m_Vertices.push_back(data[0]);
    m_Vertices.push_back(data[1]);
    m_Vertices.push_back(data[6]);

    m_Vertices.push_back(data[6]);
    m_Vertices.push_back(data[7]);
    m_Vertices.push_back(data[0]);

    // Face Basse
    m_Vertices.push_back(data[2]);
    m_Vertices.push_back(data[3]);
    m_Vertices.push_back(data[5]);

    m_Vertices.push_back(data[3]);
    m_Vertices.push_back(data[4]);
    m_Vertices.push_back(data[5]);

    // Face Gauche
    m_Vertices.push_back(data[0]);
    m_Vertices.push_back(data[3]);
    m_Vertices.push_back(data[4]);

    m_Vertices.push_back(data[0]);
    m_Vertices.push_back(data[4]);
    m_Vertices.push_back(data[7]);

    // Face Droite
    m_Vertices.push_back(data[1]);
    m_Vertices.push_back(data[2]);
    m_Vertices.push_back(data[5]);

    m_Vertices.push_back(data[1]);
    m_Vertices.push_back(data[5]);
    m_Vertices.push_back(data[6]);
}

}
