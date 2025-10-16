#include "VisualNode.h"

#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

VisualNode::VisualNode(const Node *node) : _node(node) {
    _nodeId = ed::NodeId(reinterpret_cast<uintptr_t>(node));

    _inputPins.reserve(_node->_inputHandles.size());
    _outputPins.reserve(_node->_outputHandles.size());

    for (size_t i = 0; i < _node->_inputHandles.size(); ++i) {
        _inputPins.push_back(ed::PinId(reinterpret_cast<uintptr_t>(_node->_inputHandles[i])));
    }
    for (size_t i = 0; i < _node->_outputHandles.size(); ++i) {
        _outputPins.push_back(ed::PinId(reinterpret_cast<uintptr_t>(_node->_outputHandles[i])));
    }
}

void VisualNode::render() const {
    ed::BeginNode(_nodeId);
    ImGui::Text("Node A");

    for (int i = 0; i < _node->_inputHandles.size(); ++i) {
        ed::BeginPin(_inputPins[i], ed::PinKind::Input);
        ImGui::Text("-> In %d", i);
        ed::EndPin();
    }
    ImGui::SameLine();

    for (int i = 0; i < _node->_outputHandles.size(); ++i) {
        ed::BeginPin(_outputPins[i], ed::PinKind::Input);
        ImGui::Text("-> In %d", i);
        ed::EndPin();
    }

    ed::EndNode();
}
