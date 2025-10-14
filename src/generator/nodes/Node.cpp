#include "Node.h"

#include "Handle.h"

Node::NodeType Node::GetType() const {
    Handle<int> handle(Handle<int>::HandleType::Input, Handle<int>::DataType::Float, 0);


    return _type;
}
