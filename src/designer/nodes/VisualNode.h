#ifndef VISUALNODE_H
#define VISUALNODE_H
#include "imgui_node_editor.h"
#include "../../generator/nodes/Node.h"


class VisualNode {
public:
    explicit VisualNode(const Node* node);
    ~VisualNode() = default;

    void render() const;
private:
    ax::NodeEditor::NodeId _nodeId;
    const Node* _node;

    std::vector<ax::NodeEditor::PinId> _inputPins = {};
    std::vector<ax::NodeEditor::PinId> _outputPins = {};
};



#endif //VISUALNODE_H
