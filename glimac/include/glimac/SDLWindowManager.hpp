#pragma once

#include <cstdint>
#include <SDL/SDL.h>
#include "glm.hpp"
/*
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
*/

namespace glimac {

class SDLWindowManager {
public:
    /*SDL_Surface *screen;
    SDL_Color fColor;
    SDL_Rect fontRect;
    TTF_Font* font;*/
    SDLWindowManager(uint32_t width, uint32_t height, const char* title);

    ~SDLWindowManager();

    bool pollEvent(SDL_Event& e);

    bool isKeyPressed(SDLKey key) const;

    // button can SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT and SDL_BUTTON_MIDDLE
    bool isMouseButtonPressed(uint32_t button) const;

    glm::ivec2 getMousePosition() const;

    void swapBuffers();

    // Return the time in seconds
    float getTime() const;

    //void printF(char *c, int x, int y);
};

}
