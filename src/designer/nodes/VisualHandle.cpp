#include "VisualHandle.h"

namespace ed = ax::NodeEditor;

VisualHandle::VisualHandle(const std::shared_ptr<generator::IHandle>& handle) : _pinId(IdManager::nextPinId()), _handle(handle) {};

[[nodiscard]] bool VisualHandle::canConnect(const VisualHandle &other) const {
    if (!_handle || !other._handle)
        return false;

    return _handle->canConnect(other.getHandle().get());
}

void VisualHandle::connectTo(const VisualHandle &other) const {
    if (canConnect(other))
        _handle->connect(other.getHandle().get());
}

void drawHandleCircle(float radius, ImU32 color) {
    ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 center = ImVec2(cursor.x + radius * 1.5f, cursor.y + ImGui::GetTextLineHeight() * 0.5f);
    drawList->AddCircleFilled(center, radius, color);
}

void VisualHandle::draw(float nodeWidth) const {
    /*ed::PinKind kind = (_handle->type() == generator::IHandle::HandleType::Input) ? ed::PinKind::Input : ed::PinKind::Output;

    ed::BeginPin(_pinId, kind);
    if (kind == ed::PinKind::Input)
        ImGui::Text("-> %s", _handle->name());
    else
        ImGui::Text("%s ->", _handle->name());
    ed::EndPin();*/

    // TODO: Instead of color for input-output, use color for data type
    // Also maybe some icon instead of a circle

    // draw pin circle
    if (_handle->type() == generator::IHandle::HandleType::Input) {
        drawHandleCircle(CIRCLE_RADIUS, IM_COL32(42, 114, 33, 255));
        ed::BeginPin(_pinId, ed::PinKind::Input); // put pin around circle
        ImGui::Dummy(ImVec2(CIRCLE_RADIUS * 3, ImGui::GetTextLineHeight())); // spacing for pin
        ed::EndPin();

        ImGui::SameLine();
        ImGui::TextUnformatted(getName());
    } else {
        float width = ImGui::CalcTextSize(getName()).x + CIRCLE_RADIUS * 6;
        ImGui::SameLine(nodeWidth - width);
        ImGui::TextUnformatted(getName());
        ImGui::SameLine();

        drawHandleCircle(CIRCLE_RADIUS, IM_COL32(180, 180, 255, 255));
        ed::BeginPin(_pinId, ed::PinKind::Output); // put pin around circle
        ImGui::Dummy(ImVec2(CIRCLE_RADIUS * 3, ImGui::GetTextLineHeight())); // spacing for pin
        ed::EndPin();
    }
}
