#ifndef GLTEXTUREHANDLE_H
#define GLTEXTUREHANDLE_H

#include "SDL3/SDL_opengl.h"
#include "imgui.h"
#include "ITextureHandle.h"
#include "TexData.h"

class GLTextureHandle final : public ITextureHandle {
public:
    GLTextureHandle() {
        glGenTextures(1, &textureId);
    }

    ~GLTextureHandle() override {
        if (textureId) glDeleteTextures(1, &textureId);
    }

    int width() override { return _width; }
    int height() override { return _height; }

    void upload(const TexData& data) override {
        _width = data.getWidth();
        _height = data.getHeight();

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     _width, _height,
                     0, GL_RGBA, GL_FLOAT, data.getData());
    }

    GLuint getTextureId() const { return textureId; }
    ImTextureID getImGuiTexture() const override {
        return static_cast<ImTextureID>(static_cast<intptr_t>(textureId));
    }

private:
    GLuint textureId = 0;
    int _width = 0;
    int _height = 0;
};

#endif //GLTEXTUREHANDLE_H
