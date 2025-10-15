#ifndef INPUTHANDLE_H
#define INPUTHANDLE_H

#include "OutputHandle.h"
#include "IDataHandle.h"
#include "IHandle.h"


template <typename T>
class InputHandle final : public IDataHandle<T> {
public:
    explicit InputHandle(const T defaultValue) : IDataHandle<T>(IHandle::HandleType::Input, defaultValue) {}
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

    void connect(OutputHandle<T>* other) {
        IHandle::_version++;
        output = other;
    }

    void disconnect() {
        IHandle::_version++;
        output = nullptr;
    }

private:
    OutputHandle<T>* output = nullptr;
};



#endif //INPUTHANDLE_H
