#ifndef NODE_H
#define NODE_H

#include "Handle.h"

class Node {
public:
    enum class NodeType {
        Input,
        Output,
        Effect,
        Math
    };

    Node(NodeType type) : _type(type) {}
    virtual ~Node() = default;

    NodeType GetType() const;

    virtual void Process();

private:
    NodeType _type;

    // TODO: Use dynamic arrays or vectors for handles
    IHandle* _inputHandles = nullptr;
    IHandle* _outputHandles = nullptr;
};



#endif //NODE_H
