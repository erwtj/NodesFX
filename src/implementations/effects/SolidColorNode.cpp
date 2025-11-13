#include "SolidColorNode.h"

#include "imgproc.h"
#include "inja.hpp"

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

    #ifdef CUDA
        float* data = solid_cuda(width, height, col);
    #else
        float* data = solid_cpu(width, height, col);
    #endif

    output->setData(TexData(data, width, height));
}

std::string SolidColorNode::generateCodeInternal() {
    std::string sizeVar = size->codeVar();
    std::string colVar = color->codeVar();
    std::string outVar = output->codeVar();

    inja::json data;
    data["size"] = sizeVar;
    data["col"] = colVar;
    data["out"] = outVar;
    data["id"] = std::to_string(id());

    const char* code = R"(
// Solid
int width_{{id}} = std::max(1, static_cast<int>({{size}}.x));
int height_{{id}} = std::max(1, static_cast<int>({{size}}.y));
#ifdef CUDA
float* outData_{{id}} = solid_cuda(width_{{id}}, height_{{id}}, {{col}});
#else
float* outData_{{id}} = solid_cpu(width_{{id}}, height_{{id}}, {{col}});
#endif
TexData {{out}} = TexData(outData_{{id}}, width_{{id}}, height_{{id}});
    )";

    return inja::render(code, data);
}
