#pragma once

#include <glimac/Sphere.hpp>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000

template <class S>
struct Context
{
    const GLuint VERTEX_ATTR_POSITION = 0; 
    const GLuint VERTEX_ATTR_NORMAL = 1; 
    const GLuint VERTEX_ATTR_TEXCOORDS = 2; 
    const GLuint VERTEX_ATTR = 3; 

    // Globals Matrix Initialisation
    glm::mat4 ProjMatrix;
    glm::mat4 globalMVMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0,0,-5));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(globalMVMatrix));

    GLuint vbo; 
    GLuint vao;

    S *m_shape = nullptr;

    Context(uint width, uint height, S *shape) :
        ProjMatrix(glm::perspective(glm::radians(70.f), width/(float)height, 0.1f, 300.f)),
        m_shape{shape}
    {
        glGenBuffers(1, &vbo); // Associate our vbo to a GL vbo
        glBindBuffer(GL_ARRAY_BUFFER, vbo); // On utilise le buffer vbo
            glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeVertex) * m_shape->getVertexCount(), m_shape->getDataPointer(), GL_STATIC_DRAW); // remplissage du buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind le buffer vbo

        glGenVertexArrays(1, &vao); // Associate our vao to a GL vao
        glBindVertexArray(vao); // On utilise l'array vao
            glEnableVertexAttribArray(VERTEX_ATTR_POSITION); 
            glEnableVertexAttribArray(VERTEX_ATTR_NORMAL);
            glEnableVertexAttribArray(VERTEX_ATTR_TEXCOORDS);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glVertexAttribPointer(
                    VERTEX_ATTR_POSITION,      // index de l'attribut à spécifier, ici POSITION
                    3,      // Nombre de composant de l'attribut (ici il y a 3 position par point, donc 3)
                    GL_FLOAT,   // type d'un élément du composant
                    GL_FALSE,   // flag: doit on normaliser
                    sizeof(ShapeVertex), // Iteration à faire pour accéder au prochain attribut
                    (void*) offsetof(ShapeVertex, position)        // Offset pour commencer à lire
                );
                glVertexAttribPointer(
                    VERTEX_ATTR_NORMAL,      // index de l'attribut à spécifier, ici POSITION
                    3,      // Nombre de composant de l'attribut (ici il y a 3 position par point, donc 3)
                    GL_FLOAT,   // type d'un élément du composant
                    GL_FALSE,   // flag: doit on normaliser
                    sizeof(ShapeVertex), // Iteration à faire pour accéder au prochain attribut
                    (void*) offsetof(ShapeVertex, normal)        // Offset pour commencer à lire
                );
                glVertexAttribPointer(
                    VERTEX_ATTR_TEXCOORDS,      // index de l'attribut à spécifier, ici POSITION
                    2,      // Nombre de composant de l'attribut (ici il y a 3 position par point, donc 3)
                    GL_FLOAT,   // type d'un élément du composant
                    GL_FALSE,   // flag: doit on normaliser
                    sizeof(ShapeVertex), // Iteration à faire pour accéder au prochain attribut
                    (void*) offsetof(ShapeVertex, texCoords)        // Offset pour commencer à lire
                );
            glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind le buffer vbo
        glBindVertexArray(0); // On utilise l'array vao
    }

    // il faut que les Shape est le meme nombre de vertex et soit de meme type
    void changeShape(S *newShape) {
        assert(m_shape->getVertexCount() == newShape->getVertexCount());
        m_shape = newShape;
    }

    void free() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
};
