#ifndef INPUTHANDLE_H
#define INPUTHANDLE_H

#include "OutputHandle.h"
#include "IDataHandle.h"
#include "IHandle.h"


template <typename T>
class InputHandle final : public IDataHandle<T> {
public:
    explicit InputHandle(Node& parent, const T defaultValue)
        : IDataHandle<T>(IHandle::HandleType::Input, defaultValue) {}
    ~InputHandle() override = default;

    T data() const override {
        if (output) {
            return output->data();
        }
        return IDataHandle<T>::data();
    }

    void setData(const T& data) override {
        IHandle::_version++;
        IDataHandle<T>::setData(data);
    }

    [[nodiscard]] uint64_t version() override {
        if (output) {
            IHandle::_version = output->version(); // Sync version with output
        }
        return IHandle::_version;
    }

    bool canConnect(IHandle &other) override {
        // Can only connect to output handles of the same type
        if (other.type() != IHandle::HandleType::Output)
            return false;

        if (other.dataType() != typeid(T))
            return false;

        return !other.checkLoop(*this);
    }

    // Unsafe connect, make sure to check canConnect before calling
    void connect(IHandle &other) override {
        this->disconnect();
        IHandle::_version++;
        output = static_cast<OutputHandle<T>*>(&other);
    }

    void disconnect() {
        IHandle::_version++;
        output = nullptr;
    }

    // Returns true if connecting would create a loop
    bool checkLoop(IHandle &other) override {
        if (&other == this)
            return true;

        if (output)
            return output->checkLoop(other);

        return false;
    }
private:
    OutputHandle<T>* output = nullptr;
};



#endif //INPUTHANDLE_H
