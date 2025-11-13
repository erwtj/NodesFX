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

std::string SubtractNode::generateCodeInternal() {
    std::string aVar = inputA->codeVar();
    std::string bVar = inputB->codeVar();
    std::string outVar = output->codeVar();

    return std::format("// Subtract \n float {} = {} - {};", outVar, aVar, bVar);
}
