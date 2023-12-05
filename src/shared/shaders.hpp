#pragma once
#include "libs.hpp"

GLuint registerNewTexture(const glimac::FilePath filePath) {
    std::unique_ptr<Image> loaded_content = loadImage(filePath);
    if (loaded_content.get() == NULL) {
        std::cerr << "The texture " << filePath << " could not be loaded, exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    GLuint result;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, loaded_content.get()->getWidth(), loaded_content.get()->getHeight(), 0, GL_RGBA, GL_FLOAT, loaded_content.get()->getPixels());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return result;
}