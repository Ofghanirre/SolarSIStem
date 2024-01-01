//
// Created by Antonin Jean on 01/01/2024.
//

#pragma once

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
    class Shape {

    public:
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