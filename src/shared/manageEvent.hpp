#pragma once
#include <glimac/TrackballCamera.hpp>
#include <glimac/SDLWindowManager.hpp>

class ManageEvent {

}
/*
while(windowManager.pollEvent(e)) {
    
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
        done = true; // Leave the loop after this iteration
    }
}
*/