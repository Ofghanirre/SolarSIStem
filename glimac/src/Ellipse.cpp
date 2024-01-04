//
// Created by Antonin Jean on 01/01/2024.
//

#include <cmath>
#include <vector>
#include <iostream>
#include "glimac/common.hpp"
#include "glimac/Ellipse.hpp"

namespace glimac {

    void Ellipse::build(GLfloat width, GLfloat height, GLsizei amountOfPoints) {
        std::vector <ShapeVertex> data;

        // Générer les vertexes
        for (int i = 0; i < amountOfPoints; ++i) {
            float angle = (2 * glm::pi<float>() * i) / amountOfPoints;
            float x = width * 0.5f * cos(angle);
            float y = height * 0.5f * sin(angle);

            ShapeVertex vertex;
            vertex.position = glm::vec3(x, y, 0.0f);
            vertex.normal = glm::vec3(0.0f, 0.0f, 1.0f);
            vertex.texCoords = glm::vec2(x / width + 0.5f, y / height + 0.5f);

            data.push_back(vertex);
        }

        GLsizei i;
        for (i = 0; i < (data.size() - 1); ++i) {
            m_Vertices.push_back(data[i]);
            m_Vertices.push_back(data[i + 1]);
        }
        m_Vertices.push_back(data[i]);
        m_Vertices.push_back(data[0]);
    }
}