#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Ring.hpp"

namespace glimac {

void Ring::build(GLfloat maxradius, GLfloat minradius, GLsizei discLat) {
    std::vector<ShapeVertex> inner_ring, outer_ring;
    float total_angle = 2 * glm::pi<float>();
    float part_angle = total_angle / discLat;
    
    for(float i = 0; i <= total_angle; i += part_angle) {
        ShapeVertex vertex;
        
        vertex.texCoords.x = 0;
        vertex.texCoords.y = 0;

        vertex.normal.x = glm::cos(i);
        vertex.normal.y = 0;
        vertex.normal.z = glm::sin(i);
        
        vertex.position = maxradius * vertex.normal;
        outer_ring.push_back(vertex);

        vertex.position = minradius * vertex.normal;
        inner_ring.push_back(vertex);

        vertex.normal.x = glm::cos(i + part_angle);
        vertex.normal.y = 0;
        vertex.normal.z = glm::sin(i + part_angle);

        vertex.position = maxradius * vertex.normal;
        outer_ring.push_back(vertex);
        vertex.position = minradius * vertex.normal;
        inner_ring.push_back(vertex);
    }

    for (int i = 0; i < inner_ring.size(); i++) {
        int j = (i+1) % inner_ring.size();
        m_Vertices.push_back(inner_ring[i]);
        m_Vertices.push_back(outer_ring[i]);
        m_Vertices.push_back(outer_ring[j]);

        m_Vertices.push_back(inner_ring[i]);
        m_Vertices.push_back(inner_ring[j]);
        m_Vertices.push_back(inner_ring[j]);
    }

    m_nVertexCount = inner_ring.size() * 6;
}

}
