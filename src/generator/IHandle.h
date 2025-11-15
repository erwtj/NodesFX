#ifndef IHANDLE_H
#define IHANDLE_H

#include <format>
#include <string>
#include <typeinfo>
#include <atomic>
#include <unordered_set>

#include "Color.h"

namespace generator {
    class IHandle {
    public:
        enum class HandleType {
            Input,
            Output
        };

        explicit IHandle(const char* name, const HandleType type) : _id(idCounter++), _name(name), _type(type)  {}
        virtual ~IHandle() = default;

        [[nodiscard]] const char* name() const { return _name; }
        [[nodiscard]] HandleType type() const { return _type; }
        [[nodiscard]] uint64_t id() const { return _id; }

        [[nodiscard]] virtual const std::type_info& dataType() const = 0;
        [[nodiscard]] virtual const char* dataTypeName() const = 0;
        [[nodiscard]] virtual const std::string dataToString() const = 0;

        [[nodiscard]] virtual uint64_t version() = 0;

        // Unsafe connect, make sure to check canConnect before calling
        virtual void disconnect() = 0;
        virtual void connect(IHandle* other) = 0;
        virtual bool canConnect(IHandle* other) = 0;

        // Returns true if connecting to target would create a loop
        virtual bool checkLoop(IHandle* target) = 0;

        // TODO: Color trait is a frontend thing really
        [[nodiscard]] virtual Color color() const = 0;

        virtual explicit operator std::string() const {
            return _name;
        }

        // Code generation
        virtual std::string generateCode(std::unordered_set<uint64_t>& processedNodes) = 0; // Handles won't generate code, but output handles will pass along to parent nodes and input handles to output handles
        virtual std::string codeVar() = 0; // Returns either the variable name to assign to or the constant value
    private:
        static inline std::atomic_uint64_t idCounter = 0;
        const uint64_t _id;
        const char* _name;
    protected:
        const HandleType _type;
        uint64_t _version = 0;
    };
}


#endif //IHANDLE_H
