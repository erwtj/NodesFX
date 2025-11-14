#ifndef OUTPUTHANDLE_H
#define OUTPUTHANDLE_H
#include <unordered_set>

#include "IDataHandle.h"
#include "IHandle.h"
#include "INode.h"

// TODO: Move implementation to .cpp for smaller header includes (goes for input handle as well)
namespace generator {
    template <typename T>
    class OutputHandle final : public IDataHandle<T> {
    public:
        explicit OutputHandle(const char* name, INode* parent, const T defaultValue)
            : IDataHandle<T>(name, IHandle::HandleType::Output, defaultValue), _parent(parent) {}
        ~OutputHandle() override = default;

        T data() const override {
            _parent->process();
            return IDataHandle<T>::data();
        }

        void setData(const T& data) override {
            IHandle::_version++;
            IDataHandle<T>::setData(data);
        }

        [[nodiscard]] uint64_t version() override {
            // When requesting a version we check versions with dfs, this is done in process.
            // Ff nothing changed, nothing processes meaning our version is correct.
            // Else node will process and call setData which will update our version for us before we can return it.
            _parent->process();
            return IHandle::_version;
        }

        bool canConnect(IHandle* other) override {
            // Can only connect to input handles of the same type
            if (other->type() != IHandle::HandleType::Input)
                return false;

            if (other->dataType() != typeid(T))
                return false;

            return other->canConnect(this);
        }

        // Unsafe connect, make sure to check canConnect before calling
        void connect(IHandle* other) override {
            // Can only connect if other is input, since other is input we let it connect to us
            other->connect(this);
        }

        void disconnect() override {
            throw std::runtime_error("OutputHandle disconnect should not be called directly.");
        }

        // Returns true if connecting would create a loop
        bool checkLoop(IHandle* target) override {
            for (const auto& inputHandle : _parent->_inputHandles) {
                if (inputHandle->checkLoop(target))
                    return true;
            }

            return false;
        }

        std::string generateCode(std::unordered_set<uint64_t>& processedNodes) override {
            return _parent->generateCode(processedNodes);
        }

        std::string codeVar() override {
            // I got too lazy to properly sanitize names so it's just the handle id fuck it
            return std::format("var{}", IHandle::id());
        }

    private:
        INode* _parent;
    };
}

#endif //OUTPUTHANDLE_H
