#ifndef HANDLE_H
#define HANDLE_H
#include "IHandle.h"


template <typename T>
class IDataHandle : public IHandle {
public:
    explicit IDataHandle(const HandleType handleType, const T defaultValue) : IHandle(handleType), _defaultValue(defaultValue) {}
    ~IDataHandle() override = default;

    virtual T data() const { return _data; }
    virtual void setData(const T& data) { _data = data; }

    virtual void reset() {
        IHandle::_version++;
        _data = _defaultValue;
    }

    [[nodiscard]] const std::type_info& dataType() const override { return typeid(T); }

protected:
    const T _defaultValue;
    T _data = _defaultValue;
};



#endif //HANDLE_H
