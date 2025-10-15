#ifndef NODE_H
#define NODE_H

#include <vector>

#include "IDataHandle.h"

class Node {
public:
    // TODO: Kind of isn't needed? This is only a thing for frontend imo
    enum class NodeType {
        Input,
        Output,
        Effect,
        Math
    };

    explicit Node(const NodeType type) : _type(type) {}
    virtual ~Node() = default;

    [[nodiscard]] NodeType type() const { return _type; }

    // TODO: Make sure that versions update
    void process() {
        if (isDirty()) { // Check if dirty (this is actually our dfs)
            processInternal();
            updateCachedVersions();
        }
    };

    std::vector<IHandle*> _inputHandles = {};
    std::vector<IHandle*> _outputHandles = {};

protected:
    NodeType _type;

    std::vector<uint64_t> _inputVersions = {};

    virtual void processInternal() = 0;

private:
    [[nodiscard]] bool isDirty() const {
        if (_inputVersions.size() != _inputHandles.size())
            return true;

        for (size_t i = 0; i < _inputHandles.size(); ++i) {
            if (_inputHandles[i]->version() != _inputVersions[i])
                return true;
        }
        return false;
    }

    void updateCachedVersions() {
        _inputVersions.resize(_inputHandles.size());
        for (size_t i = 0; i < _inputHandles.size(); ++i) {
            auto* handle = _inputHandles[i];
            _inputVersions[i] = handle ? handle->version() : 0;
        }
    }
};



#endif //NODE_H
