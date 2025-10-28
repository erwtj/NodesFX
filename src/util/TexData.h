#ifndef TEXDATA_H
#define TEXDATA_H

#include <memory>
#include "SDL3/SDL_opengl.h"
#include "imgui.h"

class TexData {
public:
    TexData() = default;

    TexData(float* data, int width, int height)
        : impl(std::make_shared<Impl>()) {
        impl->setData(data, width, height);
    }

    // Copyable and assignable by default (shared_ptr does reference counting)
    TexData(const TexData&) = default;
    TexData& operator=(const TexData&) = default;

    // Accessors
    [[nodiscard]] float* getData() const { return impl ? impl->data.get() : nullptr; }
    [[nodiscard]] int getWidth() const { return impl ? impl->width : 0; }
    [[nodiscard]] int getHeight() const { return impl ? impl->height : 0; }
    [[nodiscard]] GLuint getTextureId() const { return impl ? impl->textureId : 0; }
    [[nodiscard]] ImTextureID getTexture() const {
        return static_cast<ImTextureID>(static_cast<intptr_t>(getTextureId()));
    }

    void setData(float* data, int width, int height) {
        if (!impl) impl = std::make_shared<Impl>();
        impl->setData(data, width, height);
    }

private:
    struct Impl {
        std::unique_ptr<float[]> data;
        int width = 0, height = 0;
        GLuint textureId = 0;

        ~Impl() {
            if (textureId) glDeleteTextures(1, &textureId);
        }

        void setData(float* newData, int w, int h) {
            data.reset(newData);  // takes ownership
            width = w;
            height = h;
            updateTexture();
        }

        void updateTexture() {
            if (textureId == 0) glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                         0, GL_RGBA, GL_FLOAT, data.get());
        }
    };

    std::shared_ptr<Impl> impl;
};


#endif //TEXDATA_H
