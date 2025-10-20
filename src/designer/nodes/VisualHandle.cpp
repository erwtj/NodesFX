#include "VisualHandle.h"

namespace ed = ax::NodeEditor;

VisualHandle::VisualHandle(IHandle* handle) : _pinId(IdManager::nextPinId()), _handle(handle) {};

// TODO: Check not loop
[[nodiscard]] bool VisualHandle::canConnect(const VisualHandle &other) const {
    if (!_handle || !other._handle)
        return false;

    return _handle->canConnect(other.getHandle());
}

void VisualHandle::connectTo(const VisualHandle &other) const {
    if (canConnect(other))
        _handle->connect(other.getHandle());
}

void VisualHandle::render() const {
    ed::PinKind kind = (_handle->type() == IHandle::HandleType::Input) ? ed::PinKind::Input : ed::PinKind::Output;

    ed::BeginPin(_pinId, kind);
    ImGui::Text("-> In %d", 0);
    ed::EndPin();
}
