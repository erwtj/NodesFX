#include "PowerNode.h"

using namespace nodes;
PowerNode::PowerNode() : INode() {
    _inputHandles.push_back(inputA);
    _inputHandles.push_back(inputB);

    _outputHandles.push_back(output);
}

void PowerNode::processInternal() {
    const float a = inputA->data();
    const float b = inputB->data();
    output->setData(std::pow(a, b));
}

std::string PowerNode::generateCodeInternal() {
    std::string aVar = inputA->codeVar();
    std::string bVar = inputB->codeVar();
    std::string outVar = output->codeVar();

    return std::format("// Power \nfloat {} = std::pow({}, {});", outVar, aVar, bVar);
}