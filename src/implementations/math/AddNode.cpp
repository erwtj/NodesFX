#include "AddNode.h"

using namespace nodes;
AddNode::AddNode() : INode() {
    _inputHandles.push_back(&inputA);
    _inputHandles.push_back(&inputB);

    _outputHandles.push_back(&output);
}

void AddNode::processInternal() {
    const float a = inputA.data();
    const float b = inputB.data();
    output.setData(a + b);
}