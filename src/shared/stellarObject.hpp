#include "libs.hpp"
#include "utils.hpp"
#include <glimac/Sphere.hpp>
#include <iostream>
#include <cassert>

using namespace glimac;


struct AStellarObject {
    Program &m_Program;

    GLint m_uMVPMatrix;
    GLint m_uMVMatrix;
    GLint m_uNormalMatrix;
    std::vector<GLuint> m_textures;
    std::vector<const GLchar*> m_texturesNames;
    std::vector<GLuint> m_texturesIds;
    std::vector<GLuint> ArchiveTextureName = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2};
    std::vector<AStellarObject*> m_satelites;

    AStellarObject(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds) : 
        m_Program{program}, m_textures{texturesIds}
    {
        assert(textures_uniform_locations.size() < 3);
        assert(textures_uniform_locations.size() == texturesIds.size());
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        for (uint i = 0; i < texturesIds.size(); i++){
            m_texturesIds.emplace_back(texturesIds[i]);
            m_texturesNames.emplace_back(textures_uniform_locations[i]);
            m_textures.emplace_back(checkValid(glGetUniformLocation(m_Program.getGLId(), textures_uniform_locations[i]), textures_uniform_locations[i]));
        }
    }

    void update() {
        m_uMVPMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix"), "uMVPMatrix");
        m_uMVMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uMVMatrix"), "uMVMatrix");
        m_uNormalMatrix = checkValid(glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix"), "uNormalMatrix");
        for (auto textureName: m_texturesNames){
            m_textures.emplace_back(checkValid(glGetUniformLocation(m_Program.getGLId(), textureName), textureName));
        }
    }
    
    
    void addSatelite(AStellarObject* satelite) {
        m_satelites.emplace_back(satelite);
    }

    
    void drawAll(glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time,
        GLuint vao, 
        Sphere sphere
    ) {
        glm::mat4 matrixPos = draw(globalMVMatrix, viewMatrix, ProjMatrix, time, vao, sphere);
        for(auto satelite : m_satelites){
            satelite->drawAll(matrixPos, viewMatrix, ProjMatrix, time, vao, sphere);
        }
    }

    void use() {
        m_Program.use();
    }

    virtual glm::mat4 draw(
        glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time, 
        GLuint vao, 
        Sphere sphere) = 0;

    virtual glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) = 0;

    glm::mat4 getOnePosMatrix(glm::mat4 globalMVMatrix, uint index, float time) {
        assert(index <= m_satelites.size());
        if(index == 0){
            return getPosMatrix(globalMVMatrix, time);
        }
        return m_satelites[index - 1]->getPosMatrix(globalMVMatrix, time);
    }
};


