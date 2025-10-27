#include "ProjectWindow.h"
#include <imgui_node_editor.h>
#include <ranges>

#include "imgui_internal.h"
#include "../generator/InputHandle.h"
#include "../implementations/NodeRegistry.h"
#include "../util/TextUtil.h"

namespace ed = ax::NodeEditor;

ProjectWindow::ProjectWindow() {
    ed::Config config;
    config.SettingsFile = "project1.json";
    _editorContext = ed::CreateEditor(&config);
}

void ProjectWindow::createNode(const NodeRegistry::Entry& entry, const ImVec2& position) {
    addNode(VisualNode::createFromRegistryEntry(entry), position);
}

void ProjectWindow::addNode(VisualNode node, const ImVec2& position) {
    const ax::NodeEditor::NodeId id = node.getNodeId();
    _nodes.insert(std::make_pair(id, std::move(node)));
    ed::SetNodePosition(id, position);
}

void ProjectWindow::deleteNode(VisualNode& node) {
    // Code is messy but its efficient and prevents errors where you're looping through a list you're deleting from
    std::unordered_set<ax::NodeEditor::PinId> pins = {};

    for (const auto& handle : node.getInputHandles()) {
        pins.emplace(handle.getPinId());
    }
    for (const auto& handle : node.getOutputHandles()) {
        pins.emplace(handle.getPinId());
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

    _nodes.erase(node.getNodeId());
}

void ProjectWindow::deleteLink(const LinkInfo& link) {
    const VisualHandle inputHandle = findHandleById(link.inputPinId);
    inputHandle.getHandle()->disconnect();
    _links.erase(link.id);
}

VisualNode* ProjectWindow::findNodeById(const ax::NodeEditor::NodeId nodeId) {
    auto it = _nodes.find(nodeId);
    if (it != _nodes.end()) {
        return &it->second;
    }
    return nullptr;
}

VisualNode* ProjectWindow::findNodeByHandleId(const ax::NodeEditor::PinId nodeId) {
    const ax::NodeEditor::NodeId parentNodeId = IdManager::nodeIdFromPinId(nodeId);
    return findNodeById(parentNodeId);
}

const VisualHandle& ProjectWindow::findHandleById(const ed::PinId pinId) {
    const VisualNode* node = findNodeByHandleId(pinId);

    for (auto& handle : node->getInputHandles()) {
        if (handle.getPinId() == pinId) {
            return handle;
        }
    }
    for (auto& handle : node->getOutputHandles()) {
        if (handle.getPinId() == pinId) {
            return handle;
        }
    }

    return VisualHandle(0, nullptr);
}

bool ProjectWindow::isLinkValid(const ed::PinId inputPinId, const ed::PinId outputPinId) {
    const VisualHandle inputHandle = findHandleById(inputPinId);
    const VisualHandle outputHandle = findHandleById(outputPinId);

    return inputHandle.canConnect(outputHandle);
}

void buildDockspace(ImGuiID dockspaceId) {
    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

    ImGuiID left_id, right_id;
    ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.75f, &left_id, &right_id);

    ImGui::DockBuilderDockWindow("Editor", left_id);
    ImGui::DockBuilderDockWindow("Inspector", right_id);
    ImGui::DockBuilderFinish(dockspaceId);
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
        for (const auto& category : NodeRegistry::categories()) {
            if (ImGui::TreeNode(category.c_str())) {
                for (const auto& nodeEntry : NodeRegistry::get(category)) {
                    if (ImGui::Selectable(nodeEntry.name.c_str())) {
                        createNode(nodeEntry, popupPos);
                    }
                }
                ImGui::TreePop();
            }
        }
    } else { // Filtered list
        bool hit = false;
        for (const auto& category : NodeRegistry::categories()) {
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

        if (!hit) {
            ImGui::TextDisabled("No results found");
        }
    }
    ImGui::Separator();

    if (_openNewNodePopup)
        ImGui::SetKeyboardFocusHere();
    ImGui::InputText("##Search", &query[0], 256);
}

void ProjectWindow::tick() {
    // Start
    ImGui::NewFrame();
    ed::SetCurrentEditor(_editorContext);

    auto& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGui::Begin("##EditorMainWindow", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus);

    // Workaround to get popup rendering ontop of dockspace and editor (since inside causes clipping errors)
    if (_openNewNodePopup) {
        ImGui::OpenPopup("New node");
    }
    if (ImGui::BeginPopup("New node")) {
        drawNodePopup();
        ImGui::EndPopup();
        _openNewNodePopup = false;
    }

    ImGuiID dockspaceId = ImGui::GetID("EditorDockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f),
        ImGuiDockNodeFlags_NoDockingInCentralNode |
        ImGuiDockNodeFlags_PassthruCentralNode);

    if (_firstTime) {
        _firstTime = false;
        buildDockspace(dockspaceId);
    }

    tickEditor();
    tickInspector();

    // End
    ImGui::End();
    ed::SetCurrentEditor(nullptr);
    ImGui::Render();
}

ImGuiWindowFlags panelFlags = ImGuiWindowFlags_NoCollapse |
                              ImGuiWindowFlags_NoMove |
                              ImGuiWindowFlags_NoResize;
void ProjectWindow::tickEditor() {
    ImGui::Begin("Editor", nullptr, panelFlags);
    ed::Begin("MyEditor", ImVec2(0.0, 0.0f));

    updateEditor();
    drawEditor();

    ed::End();
    ImGui::End();
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
                        VisualHandle inputHandle = findHandleById(inputPinId);
                        VisualHandle outputHandle = findHandleById(outputPinId);

                        outputHandle.connectTo(inputHandle);

                        // Actually check which one is input and which one is output
                        if (inputHandle.getHandle()->type() != generator::IHandle::HandleType::Input) {
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
                const auto it = _links.find(linkId);
                if (it != _links.end()) {
                    const LinkInfo& link = it->second;
                    deleteLink(link);
                }
            }
        }

        ed::NodeId nodeId;
        while (ed::QueryDeletedNode(&nodeId))  {
            if (ed::AcceptDeletedItem()) {
                auto nodeIt = _nodes.find(nodeId);
                if (nodeIt != _nodes.end()) {
                    VisualNode& node = nodeIt->second;
                    deleteNode(node);
                }
            }
        }
    }
    ed::EndDelete();
}


void ProjectWindow::drawEditor() const {
    for (const auto& [nodeId, node] : _nodes) {
        node.draw();
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

void ProjectWindow::drawInspector() {
    ImGui::Begin("Inspector", nullptr, panelFlags);

    if (const ed::NodeId hoveredNodeId = ed::GetHoveredNode()) {
        const VisualNode* node = findNodeById(hoveredNodeId);
        ImGui::Text("%s Node ID: %d", node->getName(), static_cast<int>(node->getNodeId().Get()));
        ImGui::Text("Input Handles:");
        for (const auto& handle : node->getInputHandles()) {
            ImGui::Text(" - %s", handle.getName());
        }
        ImGui::Text("Output Handles:");
        for (const auto& handle : node->getOutputHandles()) {
            ImGui::Text(" - %s", handle.getName());
        }
    } else {
        ImGui::Text("No node selected");
    }

    ImGui::End();
}

