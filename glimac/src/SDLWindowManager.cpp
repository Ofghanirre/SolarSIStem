#include "glimac/SDLWindowManager.hpp"
#include <iostream>

namespace glimac {

SDLWindowManager::SDLWindowManager(uint32_t width, uint32_t height, const char* title) {
    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    SDL_Surface *screen = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    if(!screen) {
        std::cerr << SDL_GetError() << std::endl;
        return;
    }
    /*Initialisation de SDL_TTF ( les lien se font mal a la compilation)
	if( TTF_Init() == -1 ) { 
        std::cerr << SDL_GetError() << std::endl;
		return; 
	} else {
        font = TTF_OpenFont("arial.ttf", 12);
        fColor.r = 255;
        fColor.g = 255;
        fColor.b = 255;
    }*/
    SDL_WM_SetCaption(title, nullptr);
}

SDLWindowManager::~SDLWindowManager() {
    SDL_Quit();
}

bool SDLWindowManager::pollEvent(SDL_Event& e) {
    return SDL_PollEvent(&e);
}

bool SDLWindowManager::isKeyPressed(SDLKey key) const {
    return SDL_GetKeyState(nullptr)[key];
}

// button can SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT and SDL_BUTTON_MIDDLE
bool SDLWindowManager::isMouseButtonPressed(uint32_t button) const {
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(button);
}

glm::ivec2 SDLWindowManager::getMousePosition() const {
    glm::ivec2 mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);
    return mousePos;
}

void SDLWindowManager::swapBuffers() {
    SDL_GL_SwapBuffers();
}

float SDLWindowManager::getTime() const {
    return 0.001f * SDL_GetTicks();
}

/* pour afficher du texte
void SDLWindowManager::printF(char *c, int x, int y){
    fontRect.x = x;
    fontRect.y = y;
    SDL_BlitSurface(TTF_RenderText_Solid(font, c, fColor), NULL, screen, &fontRect);
    SDL_Flip(screen);
}*/

}
