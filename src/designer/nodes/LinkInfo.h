#ifndef LINKINFO_H
#define LINKINFO_H
#include "imgui_node_editor.h"

struct LinkInfo {
    ax::NodeEditor::LinkId id;
    ax::NodeEditor::PinId inputPinId;
    ax::NodeEditor::PinId outputPinId;
};

#endif //LINKINFO_H
