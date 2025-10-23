#include "SplitVectorNode.h"

using namespace nodes;
SplitVectorNode::SplitVectorNode() : INode() {
    _inputHandles.push_back(input);

    _outputHandles.push_back(outputX);
    _outputHandles.push_back(outputY);
    _outputHandles.push_back(outputZ);
    _outputHandles.push_back(outputW);
}

void SplitVectorNode::processInternal() {
    const Vec4 vec = input->data();
    outputX->setData(vec.x);
    outputY->setData(vec.y);
    outputZ->setData(vec.z);
    outputW->setData(vec.w);
}