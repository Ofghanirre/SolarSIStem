#pragma once
#include "libs.hpp"

struct Vertex2DUV {
    glm::vec2 position;
    glm::vec2 texture_coords;

    Vertex2DUV() {}

    Vertex2DUV(glm::vec2 position_, glm::vec2 texture_coords_): position(position_), texture_coords(texture_coords_) {}
};

GLint checkValid(GLint input, const char* name) {
    if (input == -1) {
        std::cerr << "Invalid GLint input for " << name << std::endl; 
    }
    return input;
}

