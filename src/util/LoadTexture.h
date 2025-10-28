#ifndef LOADTEXTURE_H
#define LOADTEXTURE_H

#include "SDL3/SDL_opengl.h"

class TextureStuff {
public:
    static ImTextureID LoadTextureFromMemory(float* data, int width, int height) {
        // Create a OpenGL texture identifier
        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Upload pixels into texture
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, data);

        return static_cast<ImTextureID>(static_cast<intptr_t>(image_texture));
    }
};

#endif //LOADTEXTURE_H
