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
    Program programSun(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/sun3D.fs.glsl"));
    Program programMercure(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/mercure3D.fs.glsl"));
    Program programVenus(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/venus3D.fs.glsl"));
    Program programEarth(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/earth3D.fs.glsl"));
    Program programMoon(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/moon3D.fs.glsl"));
    Program programMars(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/mars3D.fs.glsl"));
    Program programJupiter(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/jupiter3D.fs.glsl"));
    Program programSaturne(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/saturne3D.fs.glsl"));
    Program programUranus(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/uranus3D.fs.glsl"));
    Program programNeptune(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/neptune3D.fs.glsl"));
    Program programPluton(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/pluton3D.fs.glsl"));

    SunProgram sunProgram(programSun, {"uSunTexture"}, {SUN_TEXTURE_ID});
    MercureProgram mercureProgram(programMercure, {"uMercureTexture"}, {MERCURE_TEXTURE_ID});
    VenusProgram venusProgram(programVenus, {"uVenusTexture"}, {VENUS_TEXTURE_ID});
    EarthProgram earthProgram(programEarth, {"uEarthTexture", "uCloudTexture"}, {EARTH_TEXTURE_ID, CLOUD_TEXTURE_ID});
    MoonProgram moonProgram(programMoon, {"uMoonTexture"}, {MOON_TEXTURE_ID});
    MarsProgram marsProgram(programMars, {"uMarsTexture"}, {MARS_TEXTURE_ID});
    JupiterProgram jupiterProgram(programJupiter, {"uJupiterTexture"}, {JUPITER_TEXTURE_ID});
    SaturneProgram saturneProgram(programSaturne, {"uSaturneTexture"}, {SATURNE_TEXTURE_ID});
    UranusProgram uranusProgram(programUranus, {"uUranusTexture"}, {URANUS_TEXTURE_ID});
    NeptuneProgram neptuneProgram(programNeptune, {"uNeptuneTexture"}, {NEPTUNE_TEXTURE_ID});
    PlutonProgram plutonProgram(programPluton, {"uPlutonTexture"}, {PLUTON_TEXTURE_ID});
    
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
    bool move = false;
    bool reset = false;
    double time = 0.f;
    uint speed = 1;
    uint focus = 0;
    while(!done) {
        // Event loop:
        done = event.exeEvent(cam_move);

        // Uniform matrix refreshing
        glClearColor(0.0, 0.0, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears le buffer et le depth buffer
        move = event.getTimeRunning();
        reset = event.getResetState();
        focus = event.getFocus();

        if (focus != 0){
            event.changeCenterCamera(sunProgram.getOnePosMatrix(ctxt.globalMVMatrix, focus, time));
        }

        sunProgram.drawAll(ctxt.globalMVMatrix, 
            event.getViewMatrix(), 
            ctxt.ProjMatrix, 
            time,
            ctxt.vao, 
            sphere
        );
        //std::cout << ctxt.globalMVMatrix << std::endl;
        
        // Update the display
        windowManager.swapBuffers();
        if ( move ) { // si le systeme doit bouger
            time += (1 * speed);
        } else if ( reset ) { // si l'on remet le systeme dans sa position original
            std::cout << "reset" << std::endl;
            time = 0.0;
            event.setResetFalse();
        }
    }
    // Application free
    ctxt.free();
    glDeleteTextures(1, &SUN_TEXTURE_ID);
    glDeleteTextures(1, &MERCURE_TEXTURE_ID);
    glDeleteTextures(1, &VENUS_TEXTURE_ID);
    glDeleteTextures(1, &EARTH_TEXTURE_ID);
    glDeleteTextures(1, &MOON_TEXTURE_ID);
    glDeleteTextures(1, &MARS_TEXTURE_ID);
    glDeleteTextures(1, &JUPITER_TEXTURE_ID);
    glDeleteTextures(1, &SATURNE_TEXTURE_ID);
    glDeleteTextures(1, &URANUS_TEXTURE_ID);
    glDeleteTextures(1, &NEPTUNE_TEXTURE_ID);
    glDeleteTextures(1, &PLUTON_TEXTURE_ID);
    return EXIT_SUCCESS;
}
