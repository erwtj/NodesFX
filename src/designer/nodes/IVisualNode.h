#ifndef IDRAWABLE_H
#define IDRAWABLE_H
#include <vector>

#include "imgui_node_editor.h"
#include "imgui_node_editor_internal.h"
#include "VisualHandle.h"

class IVisualNode {
public:
    explicit IVisualNode() : _nodeId(IdManager::nextNodeId()) {}
    virtual ~IVisualNode() = default;

    virtual void draw() = 0;

    [[nodiscard]] virtual const char* getName() const = 0;

    [[nodiscard]] virtual const std::vector<std::shared_ptr<VisualHandle>>& getInputHandles() const = 0;
    [[nodiscard]] virtual const std::vector<std::shared_ptr<VisualHandle>>& getOutputHandles() const = 0;

    [[nodiscard]] virtual ax::NodeEditor::NodeId getNodeId() const {;
        return _nodeId;
    }

protected:
    const ax::NodeEditor::NodeId _nodeId;
};

#endif //IDRAWABLE_H
