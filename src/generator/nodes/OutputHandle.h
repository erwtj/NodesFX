#ifndef OUTPUTHANDLE_H
#define OUTPUTHANDLE_H
#include "IDataHandle.h"
#include "IHandle.h"
#include "Node.h"


template <typename T>
class OutputHandle final : public IDataHandle<T> {
public:
    explicit OutputHandle(Node* parent, const T defaultValue)
        : IDataHandle<T>(IHandle::HandleType::Output, defaultValue), _parent(parent) {}
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

private:
    Node *_parent = nullptr;
};



#endif //OUTPUTHANDLE_H
