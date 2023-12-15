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
    GLuint EARTH_TEXTURE_ID, MOON_TEXTURE_ID, CLOUD_TEXTURE_ID, SUN_TEXTURE_ID, MERCURE_TEXTURE_ID, VENUS_TEXTURE_ID,
    MARS_TEXTURE_ID, JUPITER_TEXTURE_ID, SATURNE_TEXTURE_ID, URANUS_TEXTURE_ID, NEPTUNE_TEXTURE_ID, PLUTON_TEXTURE_ID;
    SUN_TEXTURE_ID = registerNewTexture("../assets/textures/SunMap.jpg");
    MERCURE_TEXTURE_ID = registerNewTexture("../assets/textures/MercuryMap.jpg");
    VENUS_TEXTURE_ID = registerNewTexture("../assets/textures/VenusMap.jpg");
    EARTH_TEXTURE_ID = registerNewTexture("../assets/textures/EarthMap.jpg");
    CLOUD_TEXTURE_ID = registerNewTexture("../assets/textures/CloudMap.jpg");
    MOON_TEXTURE_ID = registerNewTexture("../assets/textures/MoonMap.jpg");
    MARS_TEXTURE_ID = registerNewTexture("../assets/textures/MarsMap.jpg");
    JUPITER_TEXTURE_ID = registerNewTexture("../assets/textures/JupiterMap.jpg");
    SATURNE_TEXTURE_ID = registerNewTexture("../assets/textures/SaturnMap.jpg");
    URANUS_TEXTURE_ID = registerNewTexture("../assets/textures/UranusMap.jpg");
    NEPTUNE_TEXTURE_ID = registerNewTexture("../assets/textures/NeptuneMap.jpg");
    PLUTON_TEXTURE_ID = registerNewTexture("../assets/textures/PlutoMap.jpg");
    std::cout << "done texture loading" << std::endl;

    // Loading & Compiling Shaders
    FilePath applicationPath(argv[0]);
    Program program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl"));

    SunProgram sunProgram(program, {"uSunTexture"}, {SUN_TEXTURE_ID});
    MercureProgram mercureProgram(program, {"uMercureTexture"}, {MERCURE_TEXTURE_ID});
    VenusProgram venusProgram(program, {"uVenusTexture"}, {VENUS_TEXTURE_ID});
    EarthProgram earthProgram(program, {"uEarthTexture", "uCloudTexture"}, {EARTH_TEXTURE_ID, CLOUD_TEXTURE_ID});
    MoonProgram moonProgram(program, {"uMoonTexture"}, {MOON_TEXTURE_ID});
    MarsProgram marsProgram(program, {"uMarsTexture"}, {MARS_TEXTURE_ID});
    JupiterProgram jupiterProgram(program, {"uJupiterTexture"}, {JUPITER_TEXTURE_ID});
    SaturneProgram saturneProgram(program, {"uSaturneTexture"}, {SATURNE_TEXTURE_ID});
    UranusProgram uranusProgram(program, {"uUranusTexture"}, {URANUS_TEXTURE_ID});
    NeptuneProgram neptuneProgram(program, {"uNeptuneTexture"}, {NEPTUNE_TEXTURE_ID});
    PlutonProgram plutonProgram(program, {"uPlutonTexture"}, {PLUTON_TEXTURE_ID});

    std::cout << "done generate planet" << std::endl;

    earthProgram.addSatelite(&moonProgram);

    sunProgram.addSatelite(&mercureProgram);
    sunProgram.addSatelite(&venusProgram);
    sunProgram.addSatelite(&earthProgram);
    sunProgram.addSatelite(&marsProgram);
    sunProgram.addSatelite(&jupiterProgram);
    sunProgram.addSatelite(&saturneProgram);
    sunProgram.addSatelite(&uranusProgram);
    sunProgram.addSatelite(&neptuneProgram);
    sunProgram.addSatelite(&plutonProgram);

    std::cout << "done link planet with satelite" << std::endl;

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

        sunProgram.drawAll(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            (move ? windowManager.getTime() : 0),
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
