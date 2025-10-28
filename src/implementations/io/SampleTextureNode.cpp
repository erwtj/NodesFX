#include "SampleTextureNode.h"

#include <iostream>

using namespace nodes;

SampleTextureNode::SampleTextureNode() : INode() {
    _outputHandles.push_back(output);
    processInternal();
}

void SampleTextureNode::processInternal() {
    auto* textureData = new float[128 * 128 * 4];
    constexpr int width = 128;
    constexpr int height = 128;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const float r = static_cast<float>(x) / static_cast<float>(width);
            const float g = static_cast<float>(y) / static_cast<float>(height);
            constexpr float b = 0.5f;
            constexpr float a = 1.0f;

            const int index = (y * width + x) * 4;
            textureData[index + 0] = r;
            textureData[index + 1] = g;
            textureData[index + 2] = b;
            textureData[index + 3] = a;
        }
    }

    TexData outTexData = output->data();
    outTexData.setData(textureData, width, height);
    output->setData(outTexData);
}
