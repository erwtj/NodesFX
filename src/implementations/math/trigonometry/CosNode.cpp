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