#include "BlurNode.h"

#ifdef CUDA
#include <cuda_runtime.h>
#endif
#include <imgproc.h>
#include <inja.hpp>

using namespace nodes;

BlurNode::BlurNode() : INode() {
    _inputHandles.push_back(inputTex);
    _inputHandles.push_back(radius);

    _outputHandles.push_back(outputTex);
}

void BlurNode::processInternal() {
    const float* inputData = inputTex->data().getData();

    if (inputData == nullptr)
    {
        // No input texture, output empty texture
        outputTex->setData(TexData());
        return;
    }

    const int width = inputTex->data().getWidth();
    const int height = inputTex->data().getHeight();
    const float r = radius->data();

    auto* outputData = new float[width * height * 4];
#ifdef CUDA
    blur_cuda(inputData, width, height, r, outputData);
#else
    blur_cpu(inputData, width, height, r, outputData);
#endif
    outputTex->setData(TexData(outputData, width, height));
}

std::string BlurNode::generateCodeInternal() {
    std::string inTexVar = inputTex->codeVar();
    std::string radiusVar = radius->codeVar();
    std::string outTexVar = outputTex->codeVar();

    inja::json data;
    data["inTex"] = inTexVar;
    data["radius"] = radiusVar;
    data["outTex"] = outTexVar;
    data["id"] = std::to_string(id()); // Any tmp types need to be tagged for this id to prevent duplicate names

    const char* code = R"(
// Blur
float* outData_{{id}} = new float[{{inTex}}.getWidth() * {{inTex}}.getHeight() * 4];
#ifdef CUDA
blur_cuda({{inTex}}.getData(), {{inTex}}.getWidth(), {{inTex}}.getHeight(), {{radius}}, outData_{{id}});
#else
blur_cpu({{inTex}}.getData(), {{inTex}}.getWidth(), {{inTex}}.getHeight(), {{radius}}, outData_{{id}});
#endif
TexData {{outTex}} = TexData(outData_{{id}}, {{inTex}}.getWidth(), {{inTex}}.getHeight());
    )";

    return inja::render(code, data);
}