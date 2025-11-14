#include "VisualHandle.h"

#include "Color.h"
#include "../../../imgproc/include/TexData.h"

namespace ed = ax::NodeEditor;

VisualHandle::VisualHandle(const ax::NodeEditor::NodeId nodeId, const std::shared_ptr<generator::IHandle>& handle) : _pinId(IdManager::nextPinId(nodeId)), _handle(handle) {};

[[nodiscard]] bool VisualHandle::canConnect(const VisualHandle* other) const {
    if (!_handle || !other->_handle)
        return false;

    return _handle->canConnect(other->getHandle().get());
}

void VisualHandle::connectTo(const VisualHandle* other) const {
    if (canConnect(other))
        _handle->connect(other->getHandle().get());
}

void drawHandleCircle(float radius, ImU32 color) {
    ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 center = ImVec2(cursor.x + radius * 1.5f, cursor.y + ImGui::GetTextLineHeight() * 0.5f);
    drawList->AddCircleFilled(center, radius, color);
}

void VisualHandle::draw(float nodeWidth) const {
    Color handleColor = _handle->color();
    ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(handleColor.r, handleColor.g, handleColor.b, handleColor.a));

    if (_handle->type() == generator::IHandle::HandleType::Input) {
        drawHandleCircle(CIRCLE_RADIUS, color);
        ed::BeginPin(_pinId, ed::PinKind::Input); // put pin around circle
        ImGui::Dummy(ImVec2(CIRCLE_RADIUS * 3, ImGui::GetTextLineHeight())); // spacing for pin
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("%s (%s) [%llu]", _handle->dataTypeName().c_str(), _handle->dataToString().c_str(), _handle->version());
        }
        ed::EndPin();

        ImGui::SameLine();
        ImGui::TextUnformatted(getName());
    } else {
        float width = ImGui::CalcTextSize(getName()).x + CIRCLE_RADIUS * 6;
        ImGui::SameLine(nodeWidth - width);
        ImGui::TextUnformatted(getName());
        ImGui::SameLine();

        drawHandleCircle(CIRCLE_RADIUS, color);
        ed::BeginPin(_pinId, ed::PinKind::Output); // put pin around circle
        ImGui::Dummy(ImVec2(CIRCLE_RADIUS * 3, ImGui::GetTextLineHeight())); // spacing for pin
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("%s (%s) [%llu]", _handle->dataTypeName().c_str(), _handle->dataToString().c_str(), _handle->version());
        }
        ed::EndPin();
    }
}
