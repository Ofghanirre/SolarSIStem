#pragma once

#include <glimac/TrackballCamera.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <SDL/SDL_events.h>

const std::vector<std::string> ViewName = {"PROFILE","HAUT","TRACBALL", "FREEFLY"};

class ManageEvent {
public:
    ManageEvent(glimac::SDLWindowManager &e, bool *upadte) : m_event(e), m_update{upadte} {};

    bool exeEvent(bool move)
    {
        if(startTime) {
            time += speed;
            *m_update = true;
        }
        bool res = false;
        SDL_Event e;
        while(m_event.pollEvent(e)) {
            if(e.type == SDL_KEYDOWN && move) {
                res = executeKey(e.key.keysym.sym, true);
            }else if (e.type == SDL_KEYUP && move) {
                m_pressed = false;
                m_keyPressed = SDLK_LAST; 
            }

            if(e.type == SDL_MOUSEBUTTONDOWN) {
                executeMouseKey(e.motion);
            }else if(e.type == SDL_MOUSEBUTTONUP) {
                m_mouse = false;
            }else if(e.type == SDL_MOUSEMOTION && m_mouse && move) {
                m_view.addRotateLeft(m_focus != 0 ? e.motion.y - lastPosY : 0);
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
        return res;
    }

    glm::mat4 getViewMatrix()
    {
        return m_view.getViewMatrix();
    }

    void changeCenterCamera(glm::mat4 PlanetPossition) {
        m_view.changeCenterCamera(PlanetPossition);
    }

    double getTime() {
        return time;
    }

    uint getFocus() {
        return m_focus;
    }

    uint getSpeed() {
        return speed;
    }

    bool getIsProfile() {
        return m_isView;
    }

    bool getDrawTraj() {
        return m_drawTraj;
    }

private:
    bool executeKey(SDLKey key, bool pres) {
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
                m_focus = m_focus == 0 ? 9 : (m_focus - 1) % 10;
                *m_update = true;
                break;
            case SDLK_RIGHT : //droite
                m_focus = (m_focus + 1) % 10;
                *m_update = true;
                break;
            case SDLK_s : // start time
                startTime = true;
                break;
            case SDLK_p : // pause time
                startTime = false;
                break;
            case SDLK_c : // quit the program
                return true;
            case SDLK_r : // reset time
                time = 0;
                startTime = false;
                *m_update = true;
                break;
            case SDLK_a : // advence time step by step
                time += startTime ? 0 : speed;
                *m_update = true;
                break;
            case SDLK_t : // draw or disable traj
                m_drawTraj = ! m_drawTraj;
                *m_update = true;
                break;
            case SDLK_SPACE : // change FOV conter on sun
                m_view.rotateLeft(m_isView ? 66.f : 0.f);
                m_isView = !m_isView; 
                *m_update = true;
                break;
            case SDLK_RETURN : // augment speed
                speed = (speed % 50) + 5;
                *m_update = true;
                break;
            default : 
                std::cout << "key :" << key << std::endl;
        };
        return false;
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
    bool startTime = false;
    bool reset = false;
    uint m_focus = 0;
    bool m_isView = true;
    double time = 0.f;
    uint speed = 5;
    bool *m_update;
    bool m_drawTraj;
};
