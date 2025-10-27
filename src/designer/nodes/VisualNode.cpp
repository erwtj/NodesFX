#include "VisualNode.h"

#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

VisualNode::VisualNode(std::unique_ptr<INode> node) : _nodeId(IdManager::nextNodeId()), _node(std::move(node)) {
    _inputHandles.reserve(_node->_inputHandles.size());
    _outputHandles.reserve(_node->_outputHandles.size());

    for (const auto& _inputHandle : _node->_inputHandles) {
        _inputHandles.emplace_back(_nodeId, _inputHandle);
    }
    for (const auto& _outputHandle : _node->_outputHandles) {
        _outputHandles.emplace_back(_nodeId, _outputHandle);
    }
}
void VisualNode::draw() const {
    ed::BeginNode(_nodeId);
    ImGui::Text("%s", _node->name());

    float handleSizeOffset = CIRCLE_RADIUS * 6;

    float largestInputHandleNameWidth = 0.0f;
    for (const auto& inputHandle : _inputHandles) {
        float nameWidth = ImGui::CalcTextSize(inputHandle.getName()).x;
        if (nameWidth > largestInputHandleNameWidth) {
            largestInputHandleNameWidth = nameWidth + handleSizeOffset;
        }
    }

    float largestOutputHandleNameWidth = 0.0f;
    for (const auto& outputHandle : _outputHandles) {
        float nameWidth = ImGui::CalcTextSize(outputHandle.getName()).x;
        if (nameWidth > largestOutputHandleNameWidth) {
            largestOutputHandleNameWidth = nameWidth + handleSizeOffset;
        }
    }

    // float handlePadding = ImGui::CalcTextSize(" ").x; // Padding between output and input handles
    float totalWidth = largestInputHandleNameWidth + largestOutputHandleNameWidth + 5.0f; // 5.0f = handlePadding

    int maxCount = std::max(_inputHandles.size(), _outputHandles.size());
    for (int i = 0; i < maxCount; i++) {
        if (i < _inputHandles.size()) {
            _inputHandles[i].draw(totalWidth);
        } else {
            ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
        }

        // ImGui::SameLine(totalWidth / 2);
        ImGui::SameLine();

        if (i < _outputHandles.size()) {
            _outputHandles[i].draw(totalWidth);
        } else {
            ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
        }
    }

    ed::EndNode();
}

VisualNode VisualNode::createFromRegistryEntry(const NodeRegistry::Entry& entry) {
    return VisualNode(entry.create());
}
