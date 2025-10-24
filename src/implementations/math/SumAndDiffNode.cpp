#include "SumAndDiffNode.h"

using namespace nodes;
SumAndDiffNode::SumAndDiffNode() : INode() {
    _inputHandles.push_back(inputA);
    _inputHandles.push_back(inputB);

    _outputHandles.push_back(outputSum);
    _outputHandles.push_back(outputDiff);
}

void SumAndDiffNode::processInternal() {
    const Vec4 vecA = inputA->data();
    const Vec4 vecB = inputB->data();

    const Vec4 vecSum = vecA + vecB;
    const Vec4 vecDiff = vecA - vecB;

    outputSum->setData(vecSum);
    outputDiff->setData(vecDiff);
}