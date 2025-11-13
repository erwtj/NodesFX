#include "MultiplyNode.h"

using namespace nodes;
MultiplyNode::MultiplyNode() : INode() {
    _inputHandles.push_back(inputA);
    _inputHandles.push_back(inputB);

    _outputHandles.push_back(output);
}

void MultiplyNode::processInternal() {
    const float a = inputA->data();
    const float b = inputB->data();
    output->setData(a * b);
}

std::string MultiplyNode::generateCodeInternal() {
    std::string aVar = inputA->codeVar();
    std::string bVar = inputB->codeVar();
    std::string outVar = output->codeVar();

    return std::format("// Multiply \nfloat {} = {} * {};", outVar, aVar, bVar);
}