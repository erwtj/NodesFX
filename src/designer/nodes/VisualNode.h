#ifndef VISUALNODE_H
#define VISUALNODE_H
#include "imgui_node_editor.h"
#include "VisualHandle.h"
#include "../../generator/INode.h"
#include "../../implementations/NodeRegistry.h"


// Node wrapper
class VisualNode {
public:
    VisualNode(VisualNode&&) noexcept = default;
    VisualNode& operator=(VisualNode&&) noexcept = delete;
    VisualNode(const VisualNode&) = delete;
    VisualNode& operator=(const VisualNode&) = delete;

    explicit VisualNode(std::unique_ptr<INode> node);
    ~VisualNode() = default;

    void draw() const;

    static VisualNode createFromRegistryEntry(const NodeRegistry::Entry& entry);

    const char* getName() const { return _node->name(); }

    [[nodiscard]] const std::vector<VisualHandle>& getInputHandles() const { return _inputHandles; }
    [[nodiscard]] const std::vector<VisualHandle>& getOutputHandles() const { return _outputHandles; }

    [[nodiscard]] ax::NodeEditor::NodeId getNodeId() const { return _nodeId; }
private:
    const ax::NodeEditor::NodeId _nodeId;
    std::unique_ptr<INode> _node;

    std::vector<VisualHandle> _inputHandles = {};
    std::vector<VisualHandle> _outputHandles = {};
};



#endif //VISUALNODE_H
