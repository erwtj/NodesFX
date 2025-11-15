#ifndef ITEXTUREHANDLE_H
#define ITEXTUREHANDLE_H

#include "TexData.h"

class ITextureHandle {
public:
    virtual ~ITextureHandle() = default;

    virtual int width() = 0;
    virtual int height() = 0;
    virtual void upload(const TexData& data) = 0;

    virtual ImTextureID getImGuiTexture() const = 0;
};

#endif //ITEXTUREHANDLE_H
