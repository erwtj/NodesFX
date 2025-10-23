#ifndef VISUALHANDLE_H
#define VISUALHANDLE_H
#include "imgui_node_editor.h"
#include "../IdManager.h"
#include "../../generator/IHandle.h"

// Pin wrapper
class VisualHandle {
public:
    explicit VisualHandle(generator::IHandle *handle);
    ~VisualHandle() = default;

    const char* getName() const { return _handle->name(); }

    [[nodiscard]] ax::NodeEditor::PinId getPinId() const { return _pinId; }
    [[nodiscard]] generator::IHandle& getHandle() const { return *_handle; }

    [[nodiscard]] bool canConnect(const VisualHandle &other) const;
    void connectTo(const VisualHandle &other) const;

    void draw() const;

private:
    const ax::NodeEditor::PinId _pinId;
    generator::IHandle *_handle = nullptr;
};



#endif //VISUALHANDLE_H
