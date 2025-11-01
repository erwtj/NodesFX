#include "SubtractNode.h"

using namespace nodes;
SubtractNode::SubtractNode() : INode() {
    _inputHandles.push_back(inputA);
    _inputHandles.push_back(inputB);

    _outputHandles.push_back(output);
}

void SubtractNode::processInternal() {
    const float a = inputA->data();
    const float b = inputB->data();
    output->setData(a - b);
}