#include "VisualNode.h"

#include "../../generator/InputHandle.h"
#include "../../generator/OutputHandle.h"

#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

VisualNode::VisualNode(std::unique_ptr<INode> node) : _node(std::move(node)) {
    _inputHandles.reserve(_node->_inputHandles.size());
    _outputHandles.reserve(_node->_outputHandles.size());

    for (const auto& _inputHandle : _node->_inputHandles) {
        _inputHandles.emplace_back(std::make_shared<VisualHandle>(_nodeId, _inputHandle));
    }
    for (const auto& _outputHandle : _node->_outputHandles) {
        _outputHandles.emplace_back(std::make_shared<VisualHandle>(_nodeId, _outputHandle));
    }
}

void VisualNode::draw() {
    ed::BeginNode(_nodeId);
    ImGui::Text("%s", _node->name());

    float handleSizeOffset = CIRCLE_RADIUS * 6;

    float largestInputHandleNameWidth = 0.0f;
    for (const auto& inputHandle : _inputHandles) {
        float nameWidth = ImGui::CalcTextSize(inputHandle->getName()).x;
        if (nameWidth > largestInputHandleNameWidth) {
            largestInputHandleNameWidth = nameWidth + handleSizeOffset;
        }
    }

    float largestOutputHandleNameWidth = 0.0f;
    for (const auto& outputHandle : _outputHandles) {
        float nameWidth = ImGui::CalcTextSize(outputHandle->getName()).x;
        if (nameWidth > largestOutputHandleNameWidth) {
            largestOutputHandleNameWidth = nameWidth + handleSizeOffset;
        }
    }

    // float handlePadding = ImGui::CalcTextSize(" ").x; // Padding between output and input handles
    float totalWidth = largestInputHandleNameWidth + largestOutputHandleNameWidth + 5.0f; // 5.0f = handlePadding
    totalWidth = std::max(totalWidth, ImGui::CalcTextSize(_node->name()).x + 20.0f); // Ensure node title fits
    totalWidth = std::max(totalWidth, 100.0f); // Minimum width

    int maxCount = std::max(_inputHandles.size(), _outputHandles.size());
    for (int i = 0; i < maxCount; i++) {
        if (i < _inputHandles.size()) {
            _inputHandles[i]->draw(totalWidth);
        } else {
            ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
        }

        // ImGui::SameLine(totalWidth / 2);
        ImGui::SameLine();

        if (i < _outputHandles.size()) {
            _outputHandles[i]->draw(totalWidth);
        } else {
            ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
        }
    }

    TexData texData;
    if (!_outputHandles.empty() && _outputHandles[0]->getHandle()->dataType() == typeid(TexData)) {
        auto texDataHandle = std::dynamic_pointer_cast<OutputHandle<TexData>>(_outputHandles[0]->getHandle());
        texData = texDataHandle->data();
    } else if (_outputHandles.empty() && _inputHandles.size() == 1 && _inputHandles[0]->getHandle()->dataType() == typeid(TexData)) {
        auto texDataHandle = std::dynamic_pointer_cast<InputHandle<TexData>>(_inputHandles[0]->getHandle());
        texData = texDataHandle->data();
    }

    if (texData.getData() != nullptr) {
        float aspectRatio = static_cast<float>(texData.getWidth()) / static_cast<float>(texData.getHeight());
        float displayWidth = totalWidth;
        float displayHeight = totalWidth / aspectRatio;
        ImGui::Image(texData.getTextureId(), ImVec2(displayWidth, displayHeight));
    }

    ed::EndNode();
}

std::unique_ptr<VisualNode> VisualNode::createFromRegistryEntry(const NodeRegistry::Entry& entry) {
    return std::make_unique<VisualNode>(entry.create());
}
