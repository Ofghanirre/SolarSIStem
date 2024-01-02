#include <GL/glew.h>
#include <iostream>
#include <stdlib.h>

// GLIMAC 
#include <glimac/SDLWindowManager.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Circle.hpp>
#include <glimac/Image.hpp>

// Shaders inclusions
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>

#include "shared/shared.hpp"
#include "stellarObjects/all.hpp"
#include "glimac/Ring.hpp"
#include "glimac/Ellipse.hpp"

using namespace glimac;

void drawData(uint focus, uint speed, double time, bool profile, bool traj) {
    //system("clear");  
    const std::vector<std::string> planetsName = {"SUN","MERCURE","VENUS","TERRE","MARS","JUPITER","SATURNE","URANUS","NEPTUNE","PLUTON"};
    std::cout << "focus : " << planetsName[focus] << std::endl
        << "speed : " << speed << std::endl
        << "days : " << time << std::endl
        << "FOV : " << (focus == 0 ? ViewName[!profile] : ViewName[2]) << std::endl
        << "Trajectoire : " << (traj ? "Oui" : "Non") << std::endl;
}

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    bool update = false;
    SDLWindowManager windowManager(WINDOW_WIDTH, WINDOW_HEIGHT, "GLImac");
    ManageEvent event(windowManager, &update);

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
    MARS_TEXTURE_ID, JUPITER_TEXTURE_ID, SATURNE_TEXTURE_ID, URANUS_TEXTURE_ID, NEPTUNE_TEXTURE_ID, PLUTON_TEXTURE_ID,
    MARS_PHOBOS_ID, MARS_DEIMOS_ID,
    DEFAULT_SATELLITE_TEXTURE_ID, SATURNE_RING_TEXTURE_ID;
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
    MARS_PHOBOS_ID = registerNewTexture("../assets/textures/MarsPhobosMap.jpg");
    MARS_DEIMOS_ID = registerNewTexture("../assets/textures/MarsDeimosMap.jpg");
    DEFAULT_SATELLITE_TEXTURE_ID = registerNewTexture("../assets/textures/DefaultSatelliteMap.jpg");
    SATURNE_RING_TEXTURE_ID = registerNewTexture("../assets/textures/SaturnRingMap.jpg");
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
    Program programSatellite(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/satellite3D.fs.glsl"));
    Program programSkyBox(loadProgram(applicationPath.dirPath() + "shaders/skybox.vs.glsl", applicationPath.dirPath() + "shaders/skybox.fs.glsl"));
    Program program2DShape(loadProgram(applicationPath.dirPath() + "shaders/circle.vs.glsl", applicationPath.dirPath() + "shaders/circle.fs.glsl"));
    Program programFilledRing(loadProgram(applicationPath.dirPath() + "shaders/ring.vs.glsl", applicationPath.dirPath() + "shaders/ring.fs.glsl"));

    RingsObject ringProgram(program2DShape);
    FilledRingsObject filledRingProgram(programFilledRing);

    SunProgram sunProgram(programSun, {"uSunTexture"}, {SUN_TEXTURE_ID});
    MercureProgram mercureProgram(programMercure, {"uMercureTexture"}, {MERCURE_TEXTURE_ID}, ringProgram);
    VenusProgram venusProgram(programVenus, {"uVenusTexture"}, {VENUS_TEXTURE_ID}, ringProgram);
    EarthProgram earthProgram(programEarth, {"uEarthTexture", "uCloudTexture"}, {EARTH_TEXTURE_ID, CLOUD_TEXTURE_ID}, ringProgram);
    MoonProgram moonProgram(programMoon, {"uMoonTexture"}, {MOON_TEXTURE_ID}, ringProgram);
    MarsProgram marsProgram(programMars, {"uMarsTexture"}, {MARS_TEXTURE_ID}, ringProgram);
    JupiterProgram jupiterProgram(programJupiter, {"uJupiterTexture"}, {JUPITER_TEXTURE_ID}, ringProgram);
    SaturneProgram saturneProgram(programSaturne, {"uSaturneTexture"}, {SATURNE_TEXTURE_ID}, ringProgram, filledRingProgram, SATURNE_RING_TEXTURE_ID);
    UranusProgram uranusProgram(programUranus, {"uUranusTexture"}, {URANUS_TEXTURE_ID}, ringProgram, filledRingProgram, SATURNE_RING_TEXTURE_ID);
    NeptuneProgram neptuneProgram(programNeptune, {"uNeptuneTexture"}, {NEPTUNE_TEXTURE_ID}, ringProgram);
    PlutonProgram plutonProgram(programPluton, {"uPlutonTexture"}, {PLUTON_TEXTURE_ID}, ringProgram);
    // MARS SATELLITES
    DeimosProgram demiosProgram(programSatellite, {"uSatelliteTexture"}, {MARS_DEIMOS_ID}, ringProgram);
    PhobosProgram phobosProgram(programSatellite, {"uSatelliteTexture"}, {MARS_PHOBOS_ID}, ringProgram);
    // JUPYTER SATELLITES
    CallistoProgram callistoProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    GanymedeProgram ganymedeProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    EuropaProgram europaProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    IOProgram ioProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    // SATURNE SATELLITES
    MimasProgram mimasProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    EnceladusProgram enceladusProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    TethysProgram tethysProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    DioneProgram dioneProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    RheaProgram rheaProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    TitanProgram titanProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    HyperionProgram hyperionProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    IapetusProgram iapetusProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    // URANUS SATELLITES
    ArielProgram arielProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    UmbrielProgram umbrielProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    TitaniaProgram titaniaProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    OberonProgram oberonProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    MirandaProgram mirandaProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    // NEPTUNE SATELITTES
    TritonProgram tritonProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    NereideProgram nereideProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);
    // PLUTON SATELITTES
    CharonProgram charonProgram(programSatellite, {"uSatelliteTexture"}, {DEFAULT_SATELLITE_TEXTURE_ID}, ringProgram);

    std::cout << "done generate planet" << std::endl;

    earthProgram.addSatelite(&moonProgram);

    marsProgram.addSatelite(&demiosProgram);
    marsProgram.addSatelite(&phobosProgram);

    jupiterProgram.addSatelite(&callistoProgram);
    jupiterProgram.addSatelite(&ganymedeProgram);
    jupiterProgram.addSatelite(&europaProgram);
    jupiterProgram.addSatelite(&ioProgram);

    saturneProgram.addSatelite(&mimasProgram);
    saturneProgram.addSatelite(&enceladusProgram);
    saturneProgram.addSatelite(&tethysProgram);
    saturneProgram.addSatelite(&dioneProgram);
    saturneProgram.addSatelite(&rheaProgram);
    saturneProgram.addSatelite(&titanProgram);
    saturneProgram.addSatelite(&hyperionProgram);
    saturneProgram.addSatelite(&iapetusProgram);

    uranusProgram.addSatelite(&arielProgram);
    uranusProgram.addSatelite(&umbrielProgram);
    uranusProgram.addSatelite(&titaniaProgram);
    uranusProgram.addSatelite(&oberonProgram);
    uranusProgram.addSatelite(&mirandaProgram);

    neptuneProgram.addSatelite(&tritonProgram);
    neptuneProgram.addSatelite(&nereideProgram);

    plutonProgram.addSatelite(&charonProgram);

    sunProgram.addSatelite(&mercureProgram);
    sunProgram.addSatelite(&venusProgram);
    sunProgram.addSatelite(&earthProgram);
    sunProgram.addSatelite(&marsProgram);
    sunProgram.addSatelite(&jupiterProgram);
    sunProgram.addSatelite(&saturneProgram);
    sunProgram.addSatelite(&uranusProgram);
    sunProgram.addSatelite(&neptuneProgram);
    sunProgram.addSatelite(&plutonProgram);

    std::cout << "done link planet with satelite " << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    std::vector<glimac::FilePath> faces
    {
        "../assets/textures/MoonMap.jpg",
        "../assets/textures/MoonMap.jpg",
        "../assets/textures/MoonMap.jpg",
        "../assets/textures/MoonMap.jpg",
        "../assets/textures/MoonMap.jpg",
        "../assets/textures/MoonMap.jpg"
    };
    Sphere sphere(1, 32, 16);
    Circle circle(2, 128);
    Ring ring(2, 1, 42);
    // ADDING DEPTH HANDLING
    glEnable(GL_DEPTH_TEST);
    GeometricalContext context(WINDOW_WIDTH, WINDOW_HEIGHT, &sphere, &circle, &ring);
    SkyBox sky(
        programSkyBox, 
        faces
    );
    sky.setContext();

    // Application loop:
    bool done = false;
    bool cam_move = true;
    double time = 0.f;
    uint speed = 3;
    uint focus;
    update = true;
    while(!done) {
        // Event loop:
        done = event.exeEvent(cam_move);

        // Uniform matrix refreshing
        glClearColor(0.0, 1.0, 0.1, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears le buffer et le depth buffer
        time = event.getTime();
        focus = event.getFocus();

        if(update){
            drawData(focus, event.getSpeed(), time, event.getIsProfile(), event.getDrawTraj());
            update = false;
        }

        if (focus != 0){
            glm::mat4 tmp = sunProgram.getOnePosMatrix(context.ctxtSphere.globalMVMatrix * event.getViewMatrix(), focus, time);
            sunProgram.drawAll(tmp, 
                glm::mat4(1.0f),
                time,
                event.getDrawTraj(),
                context,
                tmp,
                event.isLightOn()
            );
        } else {
            sunProgram.drawAll(context.ctxtSphere.globalMVMatrix, 
                event.getViewMatrix(), 
                time,
                event.getDrawTraj(),
                context,
                context.ctxtSphere.globalMVMatrix,
                event.isLightOn()
            );
        }

        sky.draw(event.getViewMatrix());

        // Update the display
        windowManager.swapBuffers();
    }
    // Application free
    context.ctxtSphere.free();
    context.ctxtCircle.free();
    context.ctxtRing.free();
    sky.free();
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
    glDeleteTextures(1, &MARS_DEIMOS_ID);
    glDeleteTextures(1, &MARS_PHOBOS_ID);
    glDeleteTextures(1, &DEFAULT_SATELLITE_TEXTURE_ID);
    glDeleteTextures(1, &SATURNE_RING_TEXTURE_ID);
    return EXIT_SUCCESS;
}
