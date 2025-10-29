#include "FloatInputNode.h"

using namespace nodes;

FloatInputNode::FloatInputNode() : INode() {
    _outputHandles.push_back(output);
}

void FloatInputNode::processInternal() {
    // Nothing to process, value is set directly on the output handle
}