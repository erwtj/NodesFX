#include "ProjectWindow.h"
#include <imgui_node_editor.h>
#include <ranges>

#include "imgui_internal.h"

namespace ed = ax::NodeEditor;

ProjectWindow::ProjectWindow() {
    ed::Config config;
    config.SettingsFile = "project1.json";
    _editorContext = ed::CreateEditor(&config);
}

void ProjectWindow::addNode(const VisualNode &node) {
    _nodes.push_back(node);
}

VisualNode ProjectWindow::findNodeById(const ax::NodeEditor::NodeId nodeId) const {
    for (const auto& node : _nodes) {
        if (node.getNodeId() == nodeId){
            return node;
        }
    }

    return VisualNode(nullptr); // Return an invalid node if not found
}

VisualHandle ProjectWindow::findHandleById(const ed::PinId pinId) const {
    // For simplicity, we assume we have a way to get all nodes
    for (const auto& node : _nodes) {
        for (const auto& handle : node.getInputHandles()) {
            if (handle.getPinId() == pinId) {
                return handle;
            }
        }
        for (const auto& handle : node.getOutputHandles()) {
            if (handle.getPinId() == pinId) {
                return handle;
            }
        }
    }

    return VisualHandle(nullptr); // Return an invalid handle if not found
}

bool ProjectWindow::isLinkValid(const ed::PinId inputPinId, const ed::PinId outputPinId) const {
    VisualHandle inputHandle = findHandleById(inputPinId);
    VisualHandle outputHandle = findHandleById(outputPinId);

    return inputHandle.canConnect(outputHandle);
}

void buildDockspace(ImGuiID dockspaceId) {
    ImGui::DockBuilderRemoveNode(dockspaceId);
    ImGui::DockBuilderAddNode(dockspaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetMainViewport()->Size);

    ImGuiID left_id, right_id;
    ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Left, 0.5f, &left_id, &right_id);

    ImGui::DockBuilderDockWindow("Editor", left_id);
    ImGui::DockBuilderDockWindow("Inspector", right_id);
    ImGui::DockBuilderFinish(dockspaceId);
}

void drawNodePopup() {
    ImGui::Text("Node creation is not implemented yet.");
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
        _openNewNodePopup = false;
        ImGui::OpenPopup("New node");
    }
    if (ImGui::BeginPopup("New node")) {
        drawNodePopup();
        ImGui::EndPopup();
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
                std::cout << "New node requested at pin " << static_cast<int>(nodePin.Get()) << std::endl;
                _openNewNodePopup = true;
            }
        }

        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId)) {
            if (inputPinId && outputPinId) {
                if (isLinkValid(inputPinId, outputPinId)) {
                    if (ed::AcceptNewItem())
                    {
                        // TODO: Gross why are we doing this lookup again
                        VisualHandle inputHandle = findHandleById(inputPinId);
                        VisualHandle outputHandle = findHandleById(outputPinId);

                        outputHandle.connectTo(inputHandle);

                        // Actually check which one is input and which one is output
                        if (inputHandle.getHandle().type() != generator::IHandle::HandleType::Input) {
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
                            _links.push_back(newLink);
                    }
                }
            }
        }
    }
    ed::EndCreate();
}


void ProjectWindow::drawEditor() const {
    for (const auto& node : _nodes) {
        node.draw();
    }

    for (const auto& link : _links) {
        ed::Link(link.id, link.inputPinId, link.outputPinId);
    }
}

void ProjectWindow::tickInspector() {
    updateInspector();
    drawInspector();
}

void ProjectWindow::updateInspector() {
}

void ProjectWindow::drawInspector() const {
    ImGui::Begin("Inspector", nullptr, panelFlags);

    const ed::NodeId hoveredNodeId = ed::GetHoveredNode();
    if (hoveredNodeId) {
        VisualNode node = findNodeById(hoveredNodeId);
        ImGui::Text("%s Node ID: %d", node.getName(), static_cast<int>(node.getNodeId().Get()));
        ImGui::Text("Input Handles: %d", static_cast<int>(node.getInputHandles().size()));
        ImGui::Text("Output Handles: %d", static_cast<int>(node.getOutputHandles().size()));
    } else {
        ImGui::Text("No node selected");
    }

    ImGui::End();
}