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

GLuint registerNewBoxTexture(const glimac::FilePath filePath) {
    std::unique_ptr<Image> loaded_content = loadImage(filePath);
    if (loaded_content.get() == NULL) {
        std::cerr << "The texture " << filePath << " could not be loaded, exiting..." << std::endl;
        exit(EXIT_FAILURE);
    }
    GLuint result;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_CUBE_MAP, result);
    for(unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, loaded_content.get()->getWidth(), loaded_content.get()->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, loaded_content.get()->getPixels());
    }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return result;
}