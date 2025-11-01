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