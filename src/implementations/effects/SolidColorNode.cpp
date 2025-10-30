#include "SolidColorNode.h"

using namespace nodes;

SolidColorNode::SolidColorNode() : INode() {
    _inputHandles.push_back(color);
    _inputHandles.push_back(size);

    _outputHandles.push_back(output);

    SolidColorNode::processInternal();
}

void SolidColorNode::processInternal() {
    Vec2 texSize = size->data();
    Color col = color->data();

    int width = std::max(1, static_cast<int>(texSize.x));
    int height = std::max(1, static_cast<int>(texSize.y));

    auto* data = new float [width * height * 4];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t index = (y * width + x) * 4;
            data[index + 0] = col.r;
            data[index + 1] = col.g;
            data[index + 2] = col.b;
            data[index + 3] = col.a;
        }
    }

    output->setData(TexData(data, width, height));
}
