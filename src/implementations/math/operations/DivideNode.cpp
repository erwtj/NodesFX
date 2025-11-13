#include "DivideNode.h"

using namespace nodes;
DivideNode::DivideNode() : INode() {
    _inputHandles.push_back(inputA);
    _inputHandles.push_back(inputB);

    _outputHandles.push_back(output);
}

void DivideNode::processInternal() {
    const float a = inputA->data();
    const float b = inputB->data();
    output->setData(a / b);
}

std::string DivideNode::generateCodeInternal() {
    std::string aVar = inputA->codeVar();
    std::string bVar = inputB->codeVar();
    std::string outVar = output->codeVar();

    return std::format("// Divide \nfloat {} = {} / {};", outVar, aVar, bVar);
}