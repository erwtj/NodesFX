#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include "../implementations/NodeRegistry.h"
#include "nodes/LinkInfo.h"
#include "nodes/VisualNode.h"

class ProjectWindow {
public:
    ProjectWindow();
    ~ProjectWindow() = default;

    void tick();

    void createNode(const NodeRegistry::Entry& entry, const ImVec2& position = ImVec2(0, 0));
    void addNode(VisualNode node, const ImVec2& position = ImVec2(0, 0));

    void deleteNode(VisualNode& node);
    void deleteLink(const LinkInfo& link);
private:
    bool _firstTime = true;
    bool _openNewNodePopup = false;
    ax::NodeEditor::EditorContext *_editorContext = nullptr;

    std::unordered_map<ax::NodeEditor::NodeId, VisualNode> _nodes = {};
    std::unordered_map<ax::NodeEditor::LinkId, LinkInfo> _links = {};

    [[nodiscard]] bool isLinkValid(ax::NodeEditor::PinId inputPinId, ax::NodeEditor::PinId outputPinId);

    void tickEditor();
    void drawEditor() const;
    void updateEditor();

    void tickInspector();
    void updateInspector();
    void drawInspector();

    void drawNodePopup();

    [[nodiscard]] VisualNode* findNodeById(ax::NodeEditor::NodeId nodeId);
    [[nodiscard]] VisualNode* findNodeByHandleId(ax::NodeEditor::PinId pinId);
    [[nodiscard]] const VisualHandle& findHandleById(ax::NodeEditor::PinId pinId);
};



#endif //PROJECTWINDOW_H
