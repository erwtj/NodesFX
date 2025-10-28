#ifndef NODE_H
#define NODE_H

#include <vector>

#include "IDataHandle.h"

namespace generator {
    class INode {
    public:
        explicit INode() = default;
        virtual ~INode() = default;

        void process() {
            if (isDirty()) { // Check if dirty (this is actually our dfs)
                processInternal();
                updateCachedVersions();
            }
        };

        [[nodiscard]] virtual const char* name() const = 0;

        std::vector<std::shared_ptr<IHandle>> _inputHandles = {};
        std::vector<std::shared_ptr<IHandle>> _outputHandles = {};

    protected:
        std::vector<uint64_t> _inputVersions = {};

        virtual void processInternal() = 0;
        [[nodiscard]] virtual bool isDirtyInternal() const {
            return false;
        }

    private:
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
