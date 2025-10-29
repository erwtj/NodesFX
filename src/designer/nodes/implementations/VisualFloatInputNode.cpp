#include "VisualFloatInputNode.h"

#include "../../../implementations/input/FloatInputNode.h"

// TODO: Create abstract way of making visual nodes for input nodes
VisualFloatInputNode::VisualFloatInputNode() : VisualNode(std::make_unique<nodes::FloatInputNode>()) {
    _outputHandle = std::dynamic_pointer_cast<generator::OutputHandle<float>>(_node->_outputHandles[0]);
}

namespace ed = ax::NodeEditor;

void VisualFloatInputNode::draw() {
    ed::BeginNode(_nodeId);
    ImGui::Text("%s", _node->name());

    float handleSizeOffset = CIRCLE_RADIUS * 6;
    float totalWidth = ImGui::CalcTextSize(_outputHandle->name()).x + handleSizeOffset;
    totalWidth = std::max(totalWidth, ImGui::CalcTextSize(_node->name()).x + 20.0f); // Ensure node title fits
    totalWidth = std::max(totalWidth, 100.0f); // Minimum width

    float value = _outputHandle->data();
    ImGui::SetNextItemWidth(totalWidth - handleSizeOffset);
    std::string label = "##" + std::to_string(static_cast<uint64_t>(_nodeId));
    if (ImGui::InputFloat(label.c_str(), &value)) {
        _outputHandle->setData(value);
    }

    ImGui::SameLine();
    _outputHandles[0].draw(totalWidth);

    ed::EndNode();
}
