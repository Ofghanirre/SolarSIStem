#include <GL/glew.h>
#include <iostream>

// GLIMAC 
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Image.hpp>

// Shaders inclusions
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "shared/shared.hpp"

using namespace glimac;


const GLuint VERTEX_ATTR_POSITION = 0;  // From shaders ./triangle.vs.glsl
const GLuint VERTEX_ATTR_NORMAL = 1;  // From shaders ./triangle.vs.glsl
const GLuint VERTEX_ATTR_TEXCOORDS = 2;  // From shaders ./triangle.vs.glsl

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT WINDOW_WIDTH

#define SATTELITE_AMOUNT 3

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(WINDOW_WIDTH, WINDOW_HEIGHT, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;
    glActiveTexture(GL_TEXTURE0);

    // Loading & Compiling Textures
    GLuint EARTH_TEXTURE_ID, MOON_TEXTURE_ID, CLOUD_TEXTURE_ID;
    EARTH_TEXTURE_ID = registerNewTexture("/home/2ing2/antonin.jean/Documents/M2/S1/graphic/GLImac-Template/TP3/assets/textures/EarthMap.jpg");
    MOON_TEXTURE_ID = registerNewTexture("/home/2ing2/antonin.jean/Documents/M2/S1/graphic/GLImac-Template/TP3/assets/textures/MoonMap.jpg");
    CLOUD_TEXTURE_ID = registerNewTexture("/home/2ing2/antonin.jean/Documents/M2/S1/graphic/GLImac-Template/TP3/assets/textures/CloudMap.jpg");
    std::cout << "done texture loading" << std::endl;

    // Loading & Compiling Shaders
    FilePath applicationPath(argv[0]);
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    Sphere sphere(1, 32, 16);
    glm::mat4 ProjMatrix, globalMVMatrix, NormalMatrix;
    // Globals Matrix Initialisation
    ProjMatrix = glm::perspective(glm::radians(70.f), WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.f);
    globalMVMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0,0,-5));
    NormalMatrix = glm::transpose(glm::inverse(globalMVMatrix));

    // ADDING DEPTH HANDLING
    glEnable(GL_DEPTH_TEST);

    // Vertex Buffer Object (VBO) initialisation
    // Feeding the data to the GPU
    GLuint vbo; 
    glGenBuffers(1, &vbo); // Associate our vbo to a GL vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // On utilise le buffer vbo
        glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeVertex) * sphere.getVertexCount(), sphere.getDataPointer(), GL_STATIC_DRAW); // remplissage du buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind le buffer vbo

    // Vertex Array Object (VAO)
    // Indicate a description of the Data for the GPU
    GLuint vao;
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


    std::vector<glm::vec3> sattelites_rotation_axis_vector;
    std::vector<glm::vec3> sattelites_initial_position_vector;
    for (int i = 0; i < SATTELITE_AMOUNT; i++) {
        sattelites_rotation_axis_vector.push_back(glm::sphericalRand(1.f)); 
        sattelites_initial_position_vector.push_back(glm::sphericalRand(2.f));
    }
    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        // Uniform matrix refreshing
        glClearColor(0.0,0.0,0.1,0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears le buffer et le depth buffer
        // DRAW first sphere (EARTH)
        earthProgram.use();
            glUniform1i(earthProgram.uEarthTexture, 0);
            glUniform1i(earthProgram.uCloudTexture, 1);

            glm::mat4 earthMVMatrix = globalMVMatrix;//glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0));
            //std::cerr<<ProjMatrix<<std::endl;
            glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(earthMVMatrix));
            glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
            glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * earthMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, EARTH_TEXTURE_ID);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, CLOUD_TEXTURE_ID);

            glBindVertexArray(vao); // On utilise l'array vao
            glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0); // On utilise l'array vao

        moonProgram.use();
            glUniform1i(moonProgram.uTexture, 0);

            glm::mat4 moonMVMatrix = glm::rotate(globalMVMatrix, windowManager.getTime(), glm::vec3(0, 1, 0));
            glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, 
                glm::value_ptr(moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, 
                glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, 
                glm::value_ptr(ProjMatrix * moonMVMatrix));

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, MOON_TEXTURE_ID);
            glBindVertexArray(vao); // On utilise l'array vao
            for (int i = 0; i < SATTELITE_AMOUNT; i++) {
                    float angle;
                    glm::mat4 MVMatrixSattelite = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5)); // Translation
                    MVMatrixSattelite = glm::rotate(MVMatrixSattelite, windowManager.getTime()/2, sattelites_rotation_axis_vector.at(i)); // Translation * Rotation
                    MVMatrixSattelite = glm::translate(MVMatrixSattelite, sattelites_initial_position_vector.at(i)); // Translation * Rotation * Translation
                    MVMatrixSattelite = glm::scale(MVMatrixSattelite, glm::vec3(0.2, 0.2, 0.2)); // Translation * Rotation * Translation * Scale
                    MVMatrixSattelite = glm::rotate(MVMatrixSattelite, windowManager.getTime(), sattelites_rotation_axis_vector.at(i)); // Translation * Rotation
                    glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix * MVMatrixSattelite));
                    glUniformMatrix4fv(moonProgram.uMVMatrix, 1, false, glm::value_ptr(MVMatrixSattelite));
                    glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
                    glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
                }
            glBindVertexArray(0); // On réinitialise l'array vao
            glBindTexture(GL_TEXTURE_2D, 0);

        // Update the display
        windowManager.swapBuffers();
    }
    // Application free
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &EARTH_TEXTURE_ID);
    glDeleteTextures(1, &MOON_TEXTURE_ID);
    return EXIT_SUCCESS;
}
