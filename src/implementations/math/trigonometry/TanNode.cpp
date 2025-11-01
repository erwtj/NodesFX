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