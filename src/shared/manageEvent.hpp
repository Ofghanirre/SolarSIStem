#pragma once
#ifndef MANAGEVENT
#define MANAGEVENT

#include <glimac/TrackballCamera.hpp>
#include <glimac/SDLWindowManager.hpp>


class ManageEvent {
public:
    ManageEvent(glimac::SDLWindowManager &e) : m_event(e)
    {
        lastPosX = m_event.getMousePosition().x;
        lastPosY = m_event.getMousePosition().y;
        lastMiddlePosY = m_event.getMousePosition().y;
        rightClick = 0;
        middleClick = 0;
    };

    bool exeEvent(bool move)
    {
        SDL_Event e;
        while(m_event.pollEvent(e)) {
            /*
            if(m_event.isMouseButtonPressed(SDL_BUTTON_RIGHT)){
                // X angle
                auto currentPosX = m_event.getMousePosition().x;
                if(rightClick == 0){
                    lastPosX = currentPosX;
                }
                view.rotateLeft(currentPosX - lastPosX);
                lastPosX = currentPosX;

                // Y angle
                auto currentPosY = m_event.getMousePosition().y;
                if(rightClick == 0){
                    lastPosY = currentPosY;
                }
                view.rotateUp(currentPosY - lastPosY);
                lastPosY = currentPosY;

                rightClick = (rightClick + 1) % 2;
            }

            if(m_event.isMouseButtonPressed(SDL_BUTTON_MIDDLE)){
                // Distance
                auto currentMiddlePosY = m_event.getMousePosition().y;
                if(middleClick == 0){
                    lastMiddlePosY = currentMiddlePosY;
                }
                view.moveFront((currentMiddlePosY - lastMiddlePosY) * 0.1);
                lastMiddlePosY = currentMiddlePosY;

                middleClick = (middleClick + 1) % 2;
            }
            */
            
            if(e.type == SDL_KEYDOWN && move) {
                
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

            if(e.type == SDL_MOUSEMOTION && move) {
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
    float lastPosX;
    float lastPosY;
    float lastMiddlePosY;
    uint rightClick = 0;
    uint middleClick = 0;
};
#endif