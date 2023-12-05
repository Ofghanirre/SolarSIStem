#pragma once

#include <iostream>
#include <GL/glew.h>
#include "glm.hpp"

namespace glimac {

    class TrackBall {
        public: 
            void moveFront(float delta) {
                m_fDistance += delta;
            }

            void rotateLeft(float degrees) {
                m_fAngleX = degrees;
            }

            void rotateUp(float degrees) {
                m_fAngleY = degrees;
            }

            void addRotateLeft(float degrees) {
                m_fAngleX = degrees;
            }

            void addRotateUp(float degrees) {
                m_fAngleY = degrees;
            }

            glm::mat4 getViewMatrix() const {
                glm::mat4 ViewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -m_fDistance)); 
                ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_fAngleX), glm::vec3(1, 0, 0)); 
                ViewMatrix = glm::rotate(ViewMatrix, glm::radians(m_fAngleY), glm::vec3(0, 1, 0)); 
                return ViewMatrix;
            }

        private:
            float m_fDistance = 0.f;
            float m_fAngleX = 0.f;
            float m_fAngleY = 0.f;
    };

}