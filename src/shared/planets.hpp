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
    std::vector<AStellarObject*> m_satellites;
    std::vector<glm::vec3> m_satellites_rotation_axis;
    std::vector<glm::vec3> m_satellites_initial_position;
    float coef_diametre;

    AStellarObject(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds, float coef_diametre) :
        m_Program{program}, m_textures{texturesIds}, coef_diametre{coef_diametre}
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
    
    
    void addSatelite(AStellarObject* satelite, glm::vec3 rotation_axis, glm::vec3 initial_position) {
        m_satellites.emplace_back(satelite);
        m_satellites_initial_position.emplace_back(initial_position);
        m_satellites_rotation_axis.emplace_back(rotation_axis);
    }

    
    void drawAll(glm::mat4 globalMVMatrix, 
        glm::mat4 viewMatrix, 
        glm::mat4 ProjMatrix, 
        float time,
        GLuint vao, 
        Sphere sphere,
        glm::vec3 initial_position,
        glm::vec3 rotation_axis
    ) {
        glm::mat4 matrixPos = draw(globalMVMatrix, viewMatrix, ProjMatrix, time, vao, sphere, initial_position, rotation_axis);
        for(uint i = 0; i < m_satellites.size(); i++){
            m_satellites[i]->drawAll(matrixPos, viewMatrix, ProjMatrix, time, vao, sphere,
                             m_satellites_initial_position[i], m_satellites_rotation_axis[i]);
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
        Sphere sphere,
        glm::vec3 initial_position,
        glm::vec3 rotation_axis)
    {
        use();
        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glUniform1i(AStellarObject::m_textures[i], i);
        }

        glm::mat4 MVMatrixPlanetView = globalMVMatrix;// * viewMatrix;
        glm::mat4 MVMatrix = glm::rotate(MVMatrixPlanetView, time, rotation_axis); // Translation * Rotation
        MVMatrix = glm::translate(MVMatrix, initial_position); // Translation * Rotation * Translation
        glm::mat4 MVMatrixPos = MVMatrix;
        MVMatrix = glm::scale(MVMatrix, glm::vec3(coef_diametre, coef_diametre, coef_diametre)); // Translation * Rotation * Translation * Scale
        MVMatrix = glm::rotate(MVMatrix, time, rotation_axis); // Translation * Rotation

        glUniformMatrix4fv(AStellarObject::m_uMVMatrix, 1, GL_FALSE,
                           glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(AStellarObject::m_uNormalMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
        glUniformMatrix4fv(AStellarObject::m_uMVPMatrix, 1, GL_FALSE,
                           glm::value_ptr(ProjMatrix * MVMatrix));

        for(uint i = 0; i < AStellarObject::m_texturesIds.size(); i++){
            glActiveTexture(AStellarObject::ArchiveTextureName[i]);
            glBindTexture(GL_TEXTURE_2D, AStellarObject::m_texturesIds[i]);
        }

        glBindVertexArray(vao); // On utilise l'array vao
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0); // On utilise l'array vao
        return MVMatrixPos;
    }
};


struct SunProgram : public AStellarObject{
    SunProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, 0.7}
    {}

    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        return globalMVMatrix;
    }

    glm::mat4 getOnePosMatrix(glm::mat4 globalMVMatrix, uint index, float time) {
        assert(index <= m_satellites.size());
        if(index == 0){
            return getPosMatrix(globalMVMatrix, time);
        }
        return m_satellites[index - 1]->getPosMatrix(globalMVMatrix, time);
    }
};


struct MercureProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.02439;
    float dist_sol = 2.06;

    MercureProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}

/*    glm::mat4 getPosMatrix(glm::mat4 globalMVMatrix, float time) {
        glm::mat4 MVMatrix = glm::rotate(globalMVMatrix, glm::radians(180.f) + time, sattelites_rotation_axis);
        MVMatrix = glm::translate(MVMatrix, sattelites_initial_position);
        MVMatrix = glm::rotate(MVMatrix, glm::radians(180.f) - time, sattelites_rotation_axis);
        return MVMatrix;
    }
*/
};


struct VenusProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.0605;
    float dist_sol = 3.86;

    VenusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};


struct EarthProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.06376;
    float dist_sol = 5.1675;

    EarthProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};


struct MoonProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.01737;
    float dist_earth = 0.03918 + 0.0605;

    MoonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};


struct MarsProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.0339;
    float dist_sol = 8.14;

    MarsProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};


struct JupiterProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.7149;
    float dist_sol = 27.80;

    JupiterProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};

struct SaturneProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.6026;
    float dist_sol = 51.14;

    SaturneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};

struct UranusProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.2555;
    float dist_sol = 102.39;
    UranusProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};

struct NeptuneProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.2476;
    float dist_sol = 161.25;
    NeptuneProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};

struct PlutonProgram : public AStellarObject {
    const float COEF_DIAMETRE = 0.0118;
    float dist_sol = 210.94;

    PlutonProgram(Program& program, std::vector<const GLchar*> textures_uniform_locations, std::vector<GLuint> texturesIds): 
    AStellarObject {program, textures_uniform_locations, texturesIds, COEF_DIAMETRE}
    {}
};

