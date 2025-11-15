#ifndef IDMANAGER_H
#define IDMANAGER_H
#include <cstdint>
#include <iostream>

#include "imgui_node_editor.h"

// Handle ID's are actually two uint32_t's
// first one is the uint32_t of the node it belongs to
// second one is the unique id within that node
// so find node by shifting right 32 bits

// TODO: Make thread safe (ts not atomic rn ngl)
class IdManager {
public:
    static ax::NodeEditor::NodeId nextNodeId() {
        return {_currentNodeId++};
    }

    static ax::NodeEditor::PinId nextPinId(ax::NodeEditor::NodeId nodeId) {
        return nextPinId(static_cast<uint32_t>(nodeId.Get()));
    }

    static ax::NodeEditor::PinId nextPinId(uint32_t nodeId) {
        uint64_t combinedId = (static_cast<uint64_t>(nodeId) << 32) | _currentPinId++;
        return {combinedId};
    }

    static ax::NodeEditor::NodeId nodeIdFromPinId(ax::NodeEditor::PinId pinId) {
        const uint64_t id = pinId.Get();
        auto nodeId = static_cast<uint32_t>(id >> 32);
        return {nodeId};
    }

    static ax::NodeEditor::LinkId nextLinkId() {
        return {_currentLinkId++};
    }

private:
    inline static uint32_t _currentNodeId = 1;
    inline static uint32_t _currentPinId = 1;
    inline static uint32_t _currentLinkId = 1;
};

namespace std {
    template<>
    struct hash<ax::NodeEditor::NodeId> {
        size_t operator()(const ax::NodeEditor::NodeId& id) const noexcept {
            return std::hash<uint32_t>()(id.Get());
        }
    };

    template<>
    struct hash<ax::NodeEditor::LinkId> {
        size_t operator()(const ax::NodeEditor::LinkId& id) const noexcept {
            return std::hash<uint32_t>()(id.Get());
        }
    };

    template<>
    struct hash<ax::NodeEditor::PinId> {
        size_t operator()(const ax::NodeEditor::PinId& id) const noexcept {
            return std::hash<uint64_t>()(id.Get());
        }
    };
}


#endif //IDMANAGER_H
