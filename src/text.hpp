// use https://learnopengl.com/In-Practice/Text-Rendering
// https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2
#pragma once

#include <iostream>
#include <map>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include "shared/shared.hpp"
#include <SDL/SDL_ttf.h>

using namespace glimac;

/// Holds all state information relevant to a character as loaded using FreeType
struct Characters {
    Program &m_program;
    TTF_Font* font;
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT));
    GLuint vao, vbo, texture;

    Characters(Program& program) : m_program{program}
    {
        m_program.use();
        glUniformMatrix4fv(glGetUniformLocation(m_program.getGLId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        font = TTF_OpenFont("../assets/fonts/BodoniFLF-Bold.ttf", 24);

        texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        //SDL_Surface * sFont = TTF_RenderText_Blended(font, message.c_str(), color);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sFont->w, sFont->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, sFont->pixels);
        
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0,1); glVertex2f(x, y);
            glTexCoord2f(1,1); glVertex2f(x + sFont->w, y);
            glTexCoord2f(1,0); glVertex2f(x + sFont->w, y + sFont->h);
            glTexCoord2f(0,0); glVertex2f(x, y + sFont->h);
        }
        glEnd();
        
        // configure VAO/VBO for texture quads
        // -----------------------------------
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    //glDeleteTextures(1, &texture);
    //SDL_FreeSurface(sFont);

    // render line of text
    // -------------------
    void renderText(std::string text, float x, float y, float scale, glm::vec3 color)
    {
        // activate corresponding render state	
        m_program.use();
        glUniform3f(glGetUniformLocation(m_program.getGLId(), "textColor"), color.x, color.y, color.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(vao);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = characters[*c];

            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};



