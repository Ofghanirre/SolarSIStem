#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Circle.hpp"

namespace glimac {

void Circle::build(GLfloat r, GLsizei discLat) {
    std::vector<ShapeVertex> data;
    float total_angle = 2 * glm::pi<float>();
    float part_angle = total_angle / discLat;
    
    for(float i = 0; i <= total_angle; i += part_angle) {
        ShapeVertex vertex;
        
        vertex.texCoords.x = 0;
        vertex.texCoords.y = 0;

        vertex.normal.x = glm::cos(i);
        vertex.normal.y = glm::sin(i);
        vertex.normal.z = 0;
        
        vertex.position = r * vertex.normal;
        
        m_Vertices.push_back(vertex);

        vertex.normal.x = glm::cos(i + part_angle);
        vertex.normal.y = glm::sin(i + part_angle);
        vertex.normal.z = 0;
        
        vertex.position = r * vertex.normal;
        
        m_Vertices.push_back(vertex);
    }

    m_nVertexCount = discLat * 2;
}

}
