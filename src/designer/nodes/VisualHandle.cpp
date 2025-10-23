#include "VisualHandle.h"

namespace ed = ax::NodeEditor;

VisualHandle::VisualHandle(generator::IHandle* handle) : _pinId(IdManager::nextPinId()), _handle(handle) {};

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

void VisualHandle::draw() const {
    ed::PinKind kind = (_handle->type() == generator::IHandle::HandleType::Input) ? ed::PinKind::Input : ed::PinKind::Output;

    ed::BeginPin(_pinId, kind);
    if (kind == ed::PinKind::Input)
        ImGui::Text("-> %s", _handle->name());
    else
        ImGui::Text("%s ->", _handle->name());
    ed::EndPin();
}
