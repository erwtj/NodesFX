#ifndef IHANDLE_H
#define IHANDLE_H
#include <typeinfo>

class IHandle {
public:
    enum class HandleType {
        Input,
        Output
    };

    explicit IHandle(const HandleType type) : _type(type) {}
    virtual ~IHandle() = default;

    [[nodiscard]] HandleType type() const { return _type; }
    [[nodiscard]] virtual const std::type_info& dataType() const = 0;

    [[nodiscard]] virtual uint64_t version() = 0;

    // Unsafe connect, make sure to check canConnect before calling
    virtual void connect(IHandle &other) = 0;
    virtual bool canConnect(IHandle &other) = 0;

    // Returns true if connecting to target would create a loop
    virtual bool checkLoop(IHandle& target) = 0;
protected:
    const HandleType _type;
    uint64_t _version = 0;

};

#endif //IHANDLE_H
