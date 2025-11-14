#ifndef HANDLE_H
#define HANDLE_H

#include "../implementations/TypeTraits.h"
#include "IHandle.h"

namespace generator {
    template <typename T>
    class IDataHandle : public IHandle {
    public:
        explicit IDataHandle(const char* name, const HandleType handleType, const T defaultValue) : IHandle(name, handleType), _defaultValue(defaultValue) {}
        ~IDataHandle() override = default;

        virtual T data() const { return _data; }
        virtual void setData(const T& data) { _data = data; }

        virtual void reset() {
            IHandle::_version++;
            _data = _defaultValue;
        }

        [[nodiscard]] const std::type_info& dataType() const override { return typeid(T); }
        [[nodiscard]] const std::string dataTypeName() const override { return typeid(T).name(); } // TODO: Not user friendly
        [[nodiscard]] const std::string dataToString() const override {
            std::ostringstream oss;
            oss << data();
            return oss.str();
        }

        [[nodiscard]] Color color() const override {
            return TypeTraits<T>::color();
        }

    protected:
        const T _defaultValue;
        T _data = _defaultValue;
    };
}

#endif //HANDLE_H
