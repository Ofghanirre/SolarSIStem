//
// Created by Antonin Jean on 02/01/2024.
//

#pragma once

namespace glimac {

// Représente une sphère discrétisée centrée en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0, 0, 1)
    class Rectangle : public Shape {
        // Alloue et construit les données (implantation dans le .cpp)
        void build(GLfloat width, GLfloat height, GLfloat depth);

    public:
        // Constructeur: alloue le tableau de données et construit les attributs des vertex
        Rectangle(GLfloat width, GLfloat height, GLfloat depth):
                m_nVertexCount(0) {
            build(width, height, depth); // Construction (voir le .cpp)
        }

    private:
        std::vector<ShapeVertex> m_Vertices;
        GLsizei m_nVertexCount; // Nombre de sommets
    };

}