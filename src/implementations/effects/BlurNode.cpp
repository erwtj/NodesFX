#include "BlurNode.h"

#ifdef CUDA
#include <cuda_runtime.h>
#endif
#include <../../../imgproc/include/imgproc.h>
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

std::string BlurNode::generateCode(std::unordered_set<uint64_t> processedNodes) {
    if (processedNodes.contains(id()))
        return ""; // Node already processed
    processedNodes.emplace(id());

    std::string inTexVar = inputTex->codeVar();
    std::string radiusVar = radius->codeVar();
    std::string outTexVar = outputTex->codeVar();

    inja::json data;
    data["inTex"] = inTexVar;
    data["radius"] = radiusVar;
    data["outTex"] = outTexVar;

    const char* code = R"(
float* inData = {{inTex}}.getData();
int width = {{inTex}}.getWidth();
int height = {{inTex}}.getHeight();

float* outData = new float[width * height * 4];

#ifdef CUDA
blur_cuda(inData, width, height, {{radius}}, outData);
#else
blur_cpu(inData, width, height, {{radius}}, outData);
#endif

TexData {{outTex}} = TexData(outData, width, height);
    )";

    return inja::render(code, data);
}