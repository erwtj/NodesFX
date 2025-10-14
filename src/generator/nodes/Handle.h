#ifndef HANDLE_H
#define HANDLE_H
#include "IHandle.h"

template <typename T>
class Handle : public IHandle {
public:
    Handle(void (onChange)(), const HandleType type, const DataType dataType, const T defaultValue) : IHandle(onChange, type, dataType), _defaultValue(defaultValue) {}

    T GetData() const { return _data; }
    virtual void SetData(const T& data) { _data = data; }

    void Reset() override { _data = _defaultValue; }
    virtual void Connect(Handle<T>* other) = 0;

private:
    const T _defaultValue;

    T _data = _defaultValue;
};



#endif //HANDLE_H
