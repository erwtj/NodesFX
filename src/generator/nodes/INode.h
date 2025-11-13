#ifndef NODE_H
#define NODE_H

#include <unordered_set>
#include <vector>
#include <atomic>

#include "IDataHandle.h"

// Nodes need extra information needed for OpenFX
// Especially input nodes, which are now a part of the designer
// InputNode should be moved to generator, and contain internal fields for OpenFX settings

namespace generator {
    class INode {
    public:
        explicit INode() : _id(idCounter++) {};
        virtual ~INode() = default;

        void process() {
            if (isDirty()) { // Check if dirty (this is actually our dfs)
                processInternal();
                updateCachedVersions();
            }
        };

        [[nodiscard]] virtual const char* name() const = 0;
        [[nodiscard]] uint64_t id() const { return _id; }

        std::vector<std::shared_ptr<IHandle>> _inputHandles = {};
        std::vector<std::shared_ptr<IHandle>> _outputHandles = {};

        // We pass processedNodes along so we don't accidentally re-add a code snippet from an already processed node
        // Handles will give us the var name that the node will populate
        virtual std::string generateCode(std::unordered_set<uint64_t> processedNodes) = 0;

    protected:
        std::vector<uint64_t> _inputVersions = {};

        virtual void processInternal() = 0;
        [[nodiscard]] virtual bool isDirtyInternal() const {
            return false;
        }

    private:
        static inline std::atomic_uint64_t idCounter = 0;
        const uint64_t _id;

        [[nodiscard]] bool isDirty() const {
            if (_inputVersions.size() != _inputHandles.size())
                return true;

            for (size_t i = 0; i < _inputHandles.size(); ++i) {
                if (_inputHandles[i]->version() != _inputVersions[i])
                    return true;
            }

            return isDirtyInternal();
        }

        void updateCachedVersions() {
            _inputVersions.resize(_inputHandles.size());
            for (size_t i = 0; i < _inputHandles.size(); ++i) {
                const auto handle = _inputHandles[i];
                _inputVersions[i] = handle ? handle->version() : 0;
            }
        }
    };
}

#endif //NODE_H
