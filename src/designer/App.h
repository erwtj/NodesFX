#ifndef APP_H
#define APP_H
#include <unordered_map>

#include "nodes/LinkInfo.h"
#include "nodes/VisualNode.h"


class App {
public:
    App() = default;
    ~App() = default;

    void update();

    void addNode(const VisualNode &node);
private:
    std::vector<VisualNode> _nodes = {};
    std::vector<LinkInfo> _links = {};

    bool isLinkValid(ax::NodeEditor::PinId inputPinId, ax::NodeEditor::PinId outputPinId);

    void drawNodes() const;
    void drawLinks() const;
    void drawInspector() const {};

    VisualNode findNodeById(ax::NodeEditor::NodeId nodeId);
    VisualHandle findHandleById(ax::NodeEditor::PinId pinId);
};



#endif //APP_H
