#ifndef IDMANAGER_H
#define IDMANAGER_H
#include <cstdint>
#include <iostream>

#include "imgui_node_editor.h"

// TODO: Make thread safe if needed
class IdManager {
public:
    static uint64_t nextId() {
        return IdManager::_currentId++;
    }

    static ax::NodeEditor::NodeId nextNodeId() {
        return {nextId()};
    }

    static ax::NodeEditor::PinId nextPinId() {
        return {nextId()};
    }

    static ax::NodeEditor::LinkId nextLinkId() {
        return {nextId()};
    }

private:
    inline static uint64_t _currentId = 1;
};


#endif //IDMANAGER_H
