#include "SampleGradientTextureNode.h"

#include <iostream>

#include "imgproc.h"
#include "inja.hpp"

using namespace nodes;

SampleGradientTextureNode::SampleGradientTextureNode() : INode() {
    _outputHandles.push_back(output);
    SampleGradientTextureNode::processInternal();
}

void SampleGradientTextureNode::processInternal() {
#ifdef CUDA
    auto* data = sample_gradient_cuda(128, 128);
#else
    auto* data = sample_gradient_cpu(128, 128);
#endif

    TexData outTexData = output->data();
    outTexData.setData(data, 128, 128);
    output->setData(outTexData);
}

std::string SampleGradientTextureNode::generateCodeInternal() {
    std::string outVar = output->codeVar();

    inja::json data;
    data["outTex"] = outVar;
    data["id"] = std::to_string(id()); // Any tmp types need to be tagged for this id to prevent duplicate names

    const char* code = R"(
// Sample gradient
#ifdef CUDA
float* outData_{{id}} = sample_gradient_cuda(128, 128);
#else
float* outData_{{id}} = sample_gradient_cpu(128, 128);
#endif
TexData {{outTex}} = TexData(outData_{{id}}, 128, 128);
    )";

    return inja::render(code, data);
}
