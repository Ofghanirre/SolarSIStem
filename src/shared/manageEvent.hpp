#pragma once

#include <glimac/TrackballCamera.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <SDL/SDL_events.h>


class ManageEvent {
public:
    ManageEvent(glimac::SDLWindowManager &e) : m_event(e) {};

    bool exeEvent(bool move)
    {
        SDL_Event e;
        while(m_event.pollEvent(e)) {
            if(e.type == SDL_KEYDOWN && move) {
                executeKey(e.key.keysym.sym, true);
            }else if (e.type == SDL_KEYUP && move) {
                m_pressed = false;
                m_keyPressed = SDLK_LAST; 
            }

            if(e.type == SDL_MOUSEBUTTONDOWN) {
                executeMouseKey(e.motion);
            }else if(e.type == SDL_MOUSEBUTTONUP) {
                m_mouse = false;
            }else if(e.type == SDL_MOUSEMOTION && m_mouse && move) {
                m_view.addRotateLeft(e.motion.y - lastPosY);
                m_view.addRotateUp(e.motion.x - lastPosX);
                lastPosX = e.motion.x;
                lastPosY = e.motion.y;
            }

            if(e.type == SDL_QUIT) {
                return true; // Leave the loop after this iteration
            }
        }
        if(m_pressed) {
            executeKey(m_keyPressed, m_pressed);
        }
        return false;
    }

    glm::mat4 getViewMatrix()
    {
        return m_view.getViewMatrix();
    }

    void changeCenterCamera(glm::mat4 PlanetPossition) {
        m_view.changeCenterCamera(PlanetPossition);
    }
private:
    void executeKey(SDLKey key, bool pres) {
        switch(key) {
            case SDLK_UP : //haut
                m_pressed = pres;
                m_keyPressed = SDLK_UP;
                m_view.moveFront(-0.1);
                break;
            case SDLK_DOWN : //bas
                m_pressed = pres;
                m_keyPressed = SDLK_DOWN;
                m_view.moveFront(0.1);
                break;
            case SDLK_LEFT : //gauche
                // change planet view
            case SDLK_RIGHT : //droite
                // change planet view
            default : 
                std::cout << "key :" << key << std::endl;
        };
    }

    void executeMouseKey(SDL_MouseMotionEvent mouse) {
        switch (mouse.state){
            case SDL_BUTTON_LEFT:
                m_mouse = true;
                lastPosX = mouse.x;
                lastPosY = mouse.y;
                break;
            case SDL_BUTTON_WHEELUP:
                m_view.moveFront(0.1);
                break;
            case SDL_BUTTON_WHEELDOWN:
                m_view.moveFront(-0.1);
                break;
            case SDL_BUTTON_RIGHT:
            default:
                std::cout << "mouse key :" << mouse.state << std::endl;
        }
    }

    glimac::SDLWindowManager &m_event;
    glimac::TrackBall m_view;
    bool m_pressed = false;
    SDLKey m_keyPressed = SDLK_LAST; 
    bool m_mouse = false;
    int lastPosX = 0;
    int lastPosY = 0;
};
