#pragma once

#include <iostream>
#include <GL/glew.h>
#include "glm.hpp"

namespace glimac {

    class TrackBall {
        public: 
            void moveFront(float delta) {
                m_delta += delta;
                //ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0.f, 0.f, delta));
            }

            void rotateLeft(float degrees) {
                m_fAngleX = degrees;
            }

            void rotateUp(float degrees) {
                m_fAngleY = degrees;
            }

            void addRotateLeft(float degrees) {
                m_fAngleX += degrees;
            }

            void addRotateUp(float degrees) {
                m_fAngleY += degrees;
            }

            glm::mat4 getViewMatrix() const {
                glm::mat4 returnViewMatrix = glm::translate(ViewMatrix, glm::vec3(0.f, 0.f, m_delta));
                returnViewMatrix = glm::rotate(returnViewMatrix, glm::radians(m_fAngleX), glm::vec3(1, 0, 0)); 
                returnViewMatrix = glm::rotate(returnViewMatrix, glm::radians(m_fAngleY), glm::vec3(0, 1, 0)); 
                return returnViewMatrix;
            }

            void changeCenterCamera(glm::mat4 PlanetPossition) {
                //std::cout << PlanetPossition << std::endl;
                ViewMatrix = PlanetPossition;
            }

        private:
            glm::mat4 ViewMatrix = glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, 0.f));
            float m_fAngleX = 0.f;
            float m_fAngleY = 0.f;
            float m_delta = 0.f;
    };

}