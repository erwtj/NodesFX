#ifndef NODELIST_H
#define NODELIST_H
#include "../../../generator/nodes/Node.h"
#include "../../../generator/nodes/implementations/AddNode.h"

namespace NodeInfo {
    enum class NodeType {
        Input,
        Output,
        Effect,
        Math
    };

    struct NodeInfo {
        const char* name;
        NodeType type;
        Node* (*createFunc)();
    };

    static const std::vector<NodeInfo> allNodes = {
        // Example entries
        {"Add", NodeType::Math, []() -> Node* { return new AddNode(); }},
        // Add more nodes here
    };
}

#endif //NODELIST_H
