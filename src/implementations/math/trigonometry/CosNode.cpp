#include "CosNode.h"

#include <cmath>

using namespace nodes;
CosNode::CosNode() : INode() {
    _inputHandles.push_back(input);
    _outputHandles.push_back(output);
}

void CosNode::processInternal() {
    const float in = input->data();
    output->setData(std::cos(in));
}

std::string CosNode::generateCodeInternal() {
    std::string inVar = input->codeVar();
    std::string outVar = output->codeVar();

    return std::format("// Cos\nfloat {} = std::cos({});", outVar, inVar);
}
