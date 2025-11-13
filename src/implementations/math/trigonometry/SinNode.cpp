#include "SinNode.h"

#include <cmath>

using namespace nodes;
SinNode::SinNode() : INode() {
    _inputHandles.push_back(input);
    _outputHandles.push_back(output);
}

void SinNode::processInternal() {
    const float in = input->data();
    output->setData(std::sin(in));
}

std::string SinNode::generateCodeInternal() {
    std::string inVar = input->codeVar();
    std::string outVar = output->codeVar();

    return std::format("// Sin\nfloat {} = std::sin({});", outVar, inVar);
}