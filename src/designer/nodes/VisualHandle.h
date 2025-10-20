#ifndef VISUALHANDLE_H
#define VISUALHANDLE_H
#include "imgui_node_editor.h"
#include "../IdManager.h"
#include "../../generator/nodes/IHandle.h"

// Pin wrapper
class VisualHandle {
public:
    explicit VisualHandle(IHandle *handle);
    ~VisualHandle() = default;

    [[nodiscard]] ax::NodeEditor::PinId getPinId() const { return _pinId; }
    [[nodiscard]] IHandle& getHandle() const { return *_handle; }

    [[nodiscard]] bool canConnect(const VisualHandle &other) const;
    void connectTo(const VisualHandle &other) const;

    void render() const;

private:
    const ax::NodeEditor::PinId _pinId;
    IHandle *_handle = nullptr;
};



#endif //VISUALHANDLE_H
