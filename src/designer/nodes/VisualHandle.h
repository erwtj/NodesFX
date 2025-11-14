#ifndef VISUALHANDLE_H
#define VISUALHANDLE_H
#include "imgui_node_editor.h"
#include "../IdManager.h"
#include "../../generator/IHandle.h"

#define CIRCLE_RADIUS 6.0f

// Pin wrapper
class VisualHandle {
public:
    explicit VisualHandle(const ax::NodeEditor::NodeId nodeId, const std::shared_ptr<generator::IHandle>& handle);
    ~VisualHandle() = default;

    [[nodiscard]] const char* getName() const { return _handle->name(); }

    [[nodiscard]] ax::NodeEditor::PinId getPinId() const { return _pinId; }
    [[nodiscard]] std::shared_ptr<generator::IHandle> getHandle() const { return _handle; }

    [[nodiscard]] bool canConnect(const VisualHandle* other) const;
    void connectTo(const VisualHandle* other) const;

    void draw(float nodeWidth) const;

private:
    const ax::NodeEditor::PinId _pinId;
    std::shared_ptr<generator::IHandle> _handle = nullptr;
};



#endif //VISUALHANDLE_H
