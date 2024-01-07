#pragma once
#include "libs.hpp"
#include "context.hpp"
#include <glimac/Sphere.hpp>
#include <glimac/Circle.hpp>
#include <glimac/Ring.hpp>


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

/**
    @brief GeometricalContext is a dataClass wrapping the used forms in 
    StellarObject displaying.

    It has been made for optimisation and code cleaning
*/
struct GeometricalContext {
    GeometricalContext(uint width, uint height, 
        Sphere *sphere, 
        Circle *circle,
        Ring *ring
    ) : ctxtSphere{width, height, sphere}, 
        ctxtCircle{width, height, circle}, 
        ctxtRing{width, height, ring} {
    }

    Context<Sphere> ctxtSphere;
    Context<Circle> ctxtCircle;
    Context<Ring> ctxtRing;
};
