#include <GL/glew.h>
#include <iostream>
#include <X11/Xlib.h>

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

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000

#define SATTELITE_AMOUNT 1

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(WINDOW_WIDTH, WINDOW_HEIGHT, "GLImac");
    ManageEvent event(windowManager);

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
    GLuint EARTH_TEXTURE_ID, MOON_TEXTURE_ID, CLOUD_TEXTURE_ID, SUN_TEXTURE_ID, MERCURE_TEXTURE_ID, 
    MARS_TEXTURE_ID, JUPITER_TEXTURE_ID, SATURNE_TEXTURE_ID, URANUS_TEXTURE_ID, NEPTUNE_TEXTURE_ID, PLUTON_TEXTURE_ID;
    EARTH_TEXTURE_ID = registerNewTexture("../assets/textures/EarthMap.jpg");
    MOON_TEXTURE_ID = registerNewTexture("../assets/textures/MoonMap.jpg");
    CLOUD_TEXTURE_ID = registerNewTexture("../assets/textures/CloudMap.jpg");
    SUN_TEXTURE_ID = registerNewTexture("../assets/textures/SunMap.jpg");
    MERCURE_TEXTURE_ID = registerNewTexture("../assets/textures/MercuryMap.jpg");
    MARS_TEXTURE_ID = registerNewTexture("../assets/textures/MarsMap.jpg");
    JUPITER_TEXTURE_ID = registerNewTexture("../assets/textures/JupiterMap.jpg");
    SATURNE_TEXTURE_ID = registerNewTexture("../assets/textures/SaturnMap.jpg");
    URANUS_TEXTURE_ID = registerNewTexture("../assets/textures/UranusMap.jpg");
    NEPTUNE_TEXTURE_ID = registerNewTexture("../assets/textures/NeptuneMap.jpg");
    PLUTON_TEXTURE_ID = registerNewTexture("../assets/textures/PlutoMap.jpg");
    std::cout << "done texture loading" << std::endl;

    // Loading & Compiling Shaders
    FilePath applicationPath(argv[0]);
    EarthProgram earthProgram(applicationPath);
    MoonProgram moonProgram(applicationPath);
    SunProgram sunProgram(applicationPath);
    MercureProgram mercureProgram(applicationPath);
    VenusProgram venusProgram(applicationPath);
    MarsProgram marsProgram(applicationPath);
    JupiterProgram jupiterProgram(applicationPath);
    SaturneProgram saturneProgram(applicationPath);
    UranusProgram uranusProgram(applicationPath);
    NeptuneProgram neptuneProgram(applicationPath);
    PlutonProgram plutonProgram(applicationPath);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    Sphere sphere(1, 32, 16);
    // ADDING DEPTH HANDLING
    glEnable(GL_DEPTH_TEST);
    Context ctxt(WINDOW_WIDTH, WINDOW_HEIGHT, sphere);


    // Application loop:
    bool done = false;
    bool cam_move = true;
    bool move = true;
    while(!done) {
        // Event loop:
        done = event.exeEvent(cam_move);

        // Uniform matrix refreshing
        glClearColor(0.0, 0.0, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears le buffer et le depth buffer

        sunProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            SUN_TEXTURE_ID,
            ctxt.vao, 
            sphere
        );

        mercureProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            MERCURE_TEXTURE_ID,
            ctxt.vao, 
            sphere
        );

        venusProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            MERCURE_TEXTURE_ID,
            ctxt.vao, 
            sphere
        );
        
        glm::mat4 earthPos = earthProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            EARTH_TEXTURE_ID, 
            CLOUD_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        moonProgram.draw(earthPos, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            ctxt.NormalMatrix, 
            (move ? windowManager.getTime() : 0), 
            MOON_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        marsProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            MARS_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        jupiterProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            JUPITER_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        saturneProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            SATURNE_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        uranusProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            URANUS_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        neptuneProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            NEPTUNE_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        plutonProgram.draw(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0), 
            PLUTON_TEXTURE_ID, 
            ctxt.vao, 
            sphere
        );

        // Update the display
        windowManager.swapBuffers();
    }
    // Application free
    ctxt.free();
    glDeleteTextures(1, &EARTH_TEXTURE_ID);
    glDeleteTextures(1, &MOON_TEXTURE_ID);
    return EXIT_SUCCESS;
}
