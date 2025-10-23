#include "VisualNode.h"

#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

VisualNode::VisualNode(std::unique_ptr<INode> node) : _nodeId(IdManager::nextNodeId()), _node(std::move(node)) {
    _inputHandles.reserve(_node->_inputHandles.size());
    _outputHandles.reserve(_node->_outputHandles.size());

    for (auto _inputHandle : _node->_inputHandles) {
        _inputHandles.emplace_back(VisualHandle(_inputHandle));
    }
    for (auto _outputHandle : _node->_outputHandles) {
        VisualHandle handle{_outputHandle};
        _outputHandles.emplace_back(handle);
    }
}

void VisualNode::draw() const {
    ed::BeginNode(_nodeId);
    ImGui::Text("%s", _node->name());

    for (auto _inputHandle : _inputHandles) {
        _inputHandle.draw();
    }

    ImGui::SameLine();

    for (auto _outputHandle : _outputHandles) {
        _outputHandle.draw();
    }

    ed::EndNode();
}

VisualNode VisualNode::createFromRegistryEntry(const NodeRegistry::Entry& entry) {
    std::unique_ptr<INode> node = entry.create();
    return VisualNode(std::move(node));
}
