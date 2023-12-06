#pragma once
#ifndef MANAGEVENT
#define MANAGEVENT

#include <glimac/TrackballCamera.hpp>
#include <glimac/SDLWindowManager.hpp>


class ManageEvent {
public:
    ManageEvent(glimac::SDLWindowManager &e) : m_event(e)
    {};

    bool exeEvent()
    {
        SDL_Event e;
        while(m_event.pollEvent(e)) {
    
            if(e.type == SDL_KEYDOWN) {
                
                switch(e.key.keysym.sym) {
                    case 273 : //haut
                        view.moveFront(0.1);
                        break;
                    case 274 : //bas
                        view.moveFront(-0.1);
                        break;
                    case 276 : //gauche
                    case 275 : //droite
                    default : 
                        std::cout << "key :" << e.key.keysym.sym << std::endl;
                };
            }

            if(e.type == SDL_MOUSEMOTION) {
                view.rotateLeft(e.motion.y);
                view.rotateUp(e.motion.x);
                //std::cout << e.motion.x << " / " << e.motion.y << std::endl;
            }

            if(e.type == SDL_QUIT) {
                return true; // Leave the loop after this iteration
            }
        }
        return false;
    }

    glm::mat4 getViewMatrix()
    {
        return view.getViewMatrix();
    }
private:
    glimac::SDLWindowManager &m_event;
    glimac::TrackBall view;
};
#endif