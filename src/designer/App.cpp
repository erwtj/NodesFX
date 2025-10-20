#include "App.h"
#include <imgui_node_editor.h>
#include <ranges>

namespace ed = ax::NodeEditor;

void App::addNode(const VisualNode &node) {
    _nodes.push_back(node);
}

VisualNode App::findNodeById(const ax::NodeEditor::NodeId nodeId) {
    for (const auto& node : _nodes) {
        if (node.getNodeId() == nodeId){
            return node;
        }
    }

    return VisualNode(nullptr); // Return an invalid node if not found
}

VisualHandle App::findHandleById(const ed::PinId pinId) {
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

bool App::isLinkValid(const ed::PinId inputPinId, const ed::PinId outputPinId) {
    VisualHandle inputHandle = findHandleById(inputPinId);
    VisualHandle outputHandle = findHandleById(outputPinId);

    return inputHandle.canConnect(outputHandle);
}

void App::update() {
    if (ed::BeginCreate()) {
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
                        if (inputHandle.getHandle().type() != IHandle::HandleType::Input) {
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

    drawNodes();
    drawLinks();
}

void App::drawNodes() const {
    for (const auto& node : _nodes) {
        node.render();
    }
}

void App::drawLinks() const {
    for (const auto& link : _links) {
        ed::Link(link.id, link.inputPinId, link.outputPinId);
    }
}

