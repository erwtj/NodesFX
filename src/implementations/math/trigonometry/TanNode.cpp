#include "TanNode.h"

#include <cmath>

using namespace nodes;
TanNode::TanNode() : INode() {
    _inputHandles.push_back(input);
    _outputHandles.push_back(output);
}

void TanNode::processInternal() {
    const float in = input->data();
    output->setData(std::tan(in));
}

std::string TanNode::generateCodeInternal() {
    std::string inVar = input->codeVar();
    std::string outVar = output->codeVar();

    return std::format("// Tan\nfloat {} = std::tan({});", outVar, inVar);
}