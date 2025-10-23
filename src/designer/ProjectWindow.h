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

    void addNode(VisualNode node);
private:
    bool _firstTime = true;
    bool _openNewNodePopup = false;
    ax::NodeEditor::EditorContext *_editorContext = nullptr;

    std::vector<VisualNode> _nodes = {};
    std::vector<LinkInfo> _links = {};

    [[nodiscard]] bool isLinkValid(ax::NodeEditor::PinId inputPinId, ax::NodeEditor::PinId outputPinId) const;

    void tickEditor();
    void drawEditor() const;
    void updateEditor();

    void tickInspector();
    void updateInspector();
    void drawInspector();

    void drawNodePopup();

    [[nodiscard]] VisualNode* findNodeById(ax::NodeEditor::NodeId nodeId);
    [[nodiscard]] const VisualHandle& findHandleById(ax::NodeEditor::PinId pinId) const;

    void createNode(const NodeRegistry::Entry& entry);
};



#endif //PROJECTWINDOW_H
