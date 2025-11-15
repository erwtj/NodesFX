#include "ProjectWindow.h"
#include <imgui_node_editor.h>
#include <ranges>
#include <utility>

#include "imgui_internal.h"
#include "../generator/InputHandle.h"
#include "../implementations/NodeRegistry.h"
#include "../util/TextUtil.h"
#include "nodes/input/InputNodeRegistry.h"

namespace ed = ax::NodeEditor;

ProjectWindow::ProjectWindow(ProjectInfo info) : _info(std::move(info)) {
    _settingsFile = std::format("project-{}.json", info.id); // Stored so c_str later doesn't get deleted when string moves out of scope
    ed::Config config;
    config.SettingsFile = _settingsFile.c_str();
    _editorContext = ed::CreateEditor(&config);
}

void ProjectWindow::createNode(const NodeRegistry::Entry& entry, const ImVec2& position) {
    addNode(VisualNode::createFromRegistryEntry(entry), position);
}

void ProjectWindow::addNode(std::unique_ptr<IVisualNode> node, const ImVec2& position) {
    const ax::NodeEditor::NodeId id = node->getNodeId();
    _nodes.insert(std::make_pair(id, std::move(node)));
    ed::SetNodePosition(id, position);
}

void ProjectWindow::deleteNode(const ax::NodeEditor::NodeId& nodeId) {
    // Code is messy but its efficient and prevents errors where you're looping through a list you're deleting from
    std::unordered_set<ax::NodeEditor::PinId> pins = {};
    const IVisualNode* node = findNodeById(nodeId);

    for (const auto& handle : node->getInputHandles()) {
        pins.emplace(handle->getPinId());
    }
    for (const auto& handle : node->getOutputHandles()) {
        pins.emplace(handle->getPinId());
    }

    std::unordered_set<ax::NodeEditor::LinkId> linksToDelete = {};
    for (const auto& [linkId, link] : _links) {
        if (pins.contains(link.inputPinId) || pins.contains(link.outputPinId)) {
            linksToDelete.emplace(linkId);
        }

        if (linksToDelete.size() == pins.size()) {
            break;
        }
    }

    for (const auto& linkId : linksToDelete) {
        deleteLink(_links.at(linkId));
    }

    _nodes.erase(node->getNodeId());
}

void ProjectWindow::deleteLink(const LinkInfo& link) {
    const VisualHandle* inputHandle = findHandleById(link.inputPinId);
    inputHandle->getHandle()->disconnect();
    _links.erase(link.id);
}

IVisualNode* ProjectWindow::findNodeById(const ax::NodeEditor::NodeId nodeId) {
    auto it = _nodes.find(nodeId);
    if (it != _nodes.end()) {
        return it->second.get();
    }
    return nullptr;
}

IVisualNode* ProjectWindow::findNodeByHandleId(const ax::NodeEditor::PinId nodeId) {
    const ax::NodeEditor::NodeId parentNodeId = IdManager::nodeIdFromPinId(nodeId);
    return findNodeById(parentNodeId);
}

VisualHandle* ProjectWindow::findHandleById(const ed::PinId pinId) {
    const IVisualNode* node = findNodeByHandleId(pinId);

    for (auto& handle : node->getInputHandles()) {
        if (handle->getPinId() == pinId) {
            return handle.get();
        }
    }
    for (auto& handle : node->getOutputHandles()) {
        if (handle->getPinId() == pinId) {
            return handle.get();
        }
    }

    return nullptr;
}

bool ProjectWindow::isLinkValid(const ed::PinId inputPinId, const ed::PinId outputPinId) {
    const VisualHandle* inputHandle = findHandleById(inputPinId);
    const VisualHandle* outputHandle = findHandleById(outputPinId);

    return inputHandle->canConnect(outputHandle);
}

void ProjectWindow::drawCategory(const std::string& category) {
    ImVec2 popupPos = ed::ScreenToCanvas(ImGui::GetCursorScreenPos());

    for (const auto& subcategory : NodeRegistry::subcategories(category)) {
        if (ImGui::TreeNode(subcategory.c_str())) {
            drawCategory(category + "/" += subcategory);
            ImGui::TreePop();
        }
    }
    for (const auto& nodeEntry : NodeRegistry::get(category)) {
        if (ImGui::Selectable(nodeEntry.name.c_str())) {
            createNode(nodeEntry, popupPos);
        }
    }
}

// Even though this is function is global, you can't switch ProjectWindows while the popup is open, so it's fine (popup is functionally globally static)
char query[256] = "";
void ProjectWindow::drawNodePopup() {
    ImVec2 popupPos = ed::ScreenToCanvas(ImGui::GetCursorScreenPos());

    if (_openNewNodePopup)
        query[0] = '\0';
    ImGui::Text("Select node...");

    ImGui::Separator();

    if (query[0] == '\0') { // List all nodes
        drawCategory("");
        if (ImGui::TreeNode("Inputs")) {
            for (const auto& [name, create] : InputNodeRegistry::entries()) {
                if (ImGui::Selectable(name.c_str())) {
                    addNode(create(), popupPos);
                }
            }
            ImGui::TreePop();
        }
    } else { // Filtered list
        bool hit = false;
        for (const auto& category : NodeRegistry::allCategoryPaths()) {
            for (const auto& nodeEntry : NodeRegistry::get(category)) {
                const char* name = nodeEntry.name.c_str();
                if (containsIgnoreCase(name, query)) {
                    if (ImGui::Selectable(name)) {
                        createNode(nodeEntry, popupPos);
                    }
                    hit = true;
                }
            }
        }

        for (const auto& inputNodeEntry : InputNodeRegistry::entries()) {
            const char* name = inputNodeEntry.name.c_str();
            if (containsIgnoreCase(name, query)) {
                if (ImGui::Selectable(name)) {
                    addNode(inputNodeEntry.create(), popupPos);
                }
                hit = true;
            }
        }

        if (!hit) {
            ImGui::TextDisabled("No results found");
        }
    }
    ImGui::Separator();

    if (_openNewNodePopup)
        ImGui::SetKeyboardFocusHere();
    ImGui::InputText("##Search", &query[0], 256);
}


void ProjectWindow::tick(ImVec2 pos, ImVec2 size) {
    ed::SetCurrentEditor(_editorContext);

    if (_openNewNodePopup) {
        ImGui::OpenPopup("New node");
    }
    if (ImGui::BeginPopup("New node")) {
        drawNodePopup();
        ImGui::EndPopup();
        _openNewNodePopup = false;
    }

    const float split_ratio = 0.7f;
    const float left_width = size.x * split_ratio;
    const float right_width = size.x - left_width;

    // === LEFT WINDOW ===
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(ImVec2(left_width, size.y));

    ImGui::Begin("Editor", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    tickEditor();
    ImGui::End();

    // === RIGHT WINDOW ===
    ImGui::SetNextWindowPos(ImVec2(pos.x + left_width, pos.y));
    ImGui::SetNextWindowSize(ImVec2(right_width, size.y));

    ImGui::Begin("Inspector", nullptr,
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    tickInspector();
    ImGui::End();

    // End
    ed::SetCurrentEditor(nullptr);
}

void ProjectWindow::tickEditor() {
    ed::Begin("MyEditor", ImVec2(0.0, 0.0f));

    updateEditor();
    drawEditor();

    ed::End();
}

void ProjectWindow::updateEditor() {
    auto& io = ImGui::GetIO();
    if (io.KeyCtrl && !io.KeyShift && !io.KeyAlt && ImGui::IsKeyPressed(ImGuiKey_Space) && !ImGui::IsItemActive() && !ImGui::IsPopupOpen("New node")) {
        _openNewNodePopup = true;
    }

    if (ed::BeginCreate()) {
        ed::PinId nodePin;
        if (ed::QueryNewNode(&nodePin)) {
            if (ed::AcceptNewItem()) {
                _openNewNodePopup = true;
            }
        }

        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId)) {
            if (inputPinId && outputPinId) {
                if (isLinkValid(inputPinId, outputPinId)) {
                    if (ed::AcceptNewItem()) {
                        // TODO: Gross why are we doing this lookup again
                        VisualHandle* inputHandle = findHandleById(inputPinId);
                        VisualHandle* outputHandle = findHandleById(outputPinId);

                        outputHandle->connectTo(inputHandle);

                        // Actually check which one is input and which one is output
                        if (inputHandle->getHandle()->type() != generator::IHandle::HandleType::Input) {
                            std::swap(inputPinId, outputPinId);
                        }

                        int linkIndex = -1;
                        for (int i = 0; i < _links.size(); i++) {
                            LinkInfo link = _links[i];
                            if (link.inputPinId == inputPinId) {
                                linkIndex = i;
                                break;
                            }
                        }

                        LinkInfo newLink = { IdManager::nextLinkId(), inputPinId, outputPinId };
                        if (linkIndex != -1)
                            _links[linkIndex] = newLink;
                        else
                            _links.insert_or_assign(newLink.id, newLink);
                    }
                }
            }
        }
    }
    ed::EndCreate();

    if (ed::BeginDelete()) {
        ed::LinkId linkId;
        while (ed::QueryDeletedLink(&linkId)) {
            if (ed::AcceptDeletedItem()) {
                auto it = _links.find(linkId);
                if (it != _links.end()) {
                    const LinkInfo& link = it->second;
                    deleteLink(link);
                }
            }
        }

        ed::NodeId nodeId;
        while (ed::QueryDeletedNode(&nodeId))  {
            if (ed::AcceptDeletedItem()) {
                if (_nodes.contains(nodeId)) {
                    deleteNode(nodeId);
                }
            }
        }
    }
    ed::EndDelete();
}


void ProjectWindow::drawEditor() const {
    for (const auto& [nodeId, node] : _nodes) {
        node->draw();
    }

    for (const auto& [linkId, link] : _links) {
        ed::Link(link.id, link.inputPinId, link.outputPinId);
    }
}

void ProjectWindow::tickInspector() {
    updateInspector();
    drawInspector();
}

void ProjectWindow::updateInspector() {
}

std::string generatedCode;
TexData testTexData = {};
void ProjectWindow::drawInspector() {
    ImGui::SeparatorText("Node Info");

    if (ed::GetSelectedObjectCount() == 1) {
        ax::NodeEditor::NodeId nodeId;
        if (ed::GetSelectedNodes(&nodeId, 1)) {
            IVisualNode* node = findNodeById(nodeId);
            ImGui::Text("%s Node ID: %d", node->getName(), static_cast<int>(node->getNodeId().Get()));
            ImGui::Text("Input Handles:");
            for (const auto& handle : node->getInputHandles()) {
                ImGui::Text(" - %s (%s)", handle->getName(), handle->getHandle()->dataToString().c_str());
            }
            ImGui::Text("Output Handles:");
            for (const auto& handle : node->getOutputHandles()) {
                ImGui::Text(" - %s (%s)", handle->getName(), handle->getHandle()->dataToString().c_str());
            }

            if (ImGui::Button("Generate Code")) {
                generatedCode = node->generateCode();
            }

            ImGui::InputTextMultiline("##generated_code", generatedCode._Unchecked_begin(), generatedCode.capacity());
        }
    }
}

