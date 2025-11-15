#ifndef INPUTHANDLE_H
#define INPUTHANDLE_H

#include "OutputHandle.h"
#include "IDataHandle.h"
#include "IHandle.h"

namespace generator {
    template <typename T>
    class InputHandle final : public IDataHandle<T> {
    public:
        explicit InputHandle(const char* name, const T defaultValue)
            : IDataHandle<T>(name, IHandle::HandleType::Input, defaultValue) {}
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
                uint64_t currentOutputVersion = output->version();

                // Detect any change in output regardless of absolute values
                if (currentOutputVersion != lastSeenOutputVersion) {
                    lastSeenOutputVersion = currentOutputVersion;
                    IHandle::_version++; // bump because input's dependency changed
                }
            }
            return IHandle::_version;
        }

        bool canConnect(IHandle* other) override {
            // Can only connect to output handles of the same type
            if (other->type() != IHandle::HandleType::Output)
                return false;

            if (other->dataType() != typeid(T))
                return false;

            return !other->checkLoop(this);
        }

        // Unsafe connect, make sure to check canConnect before calling
        void connect(IHandle* other) override {
            this->disconnect();
            output = static_cast<OutputHandle<T>*>(other);
            lastSeenOutputVersion = output->version();
            IHandle::_version++;
        }

        void disconnect() override {
            if (output) {
                output = nullptr;
                lastSeenOutputVersion = 0;
                IHandle::_version++;
            }
        }

        // Returns true if connecting would create a loop
        bool checkLoop(IHandle* target) override {
            if (target == this)
                return true;

            if (output)
                return output->checkLoop(target);

            return false;
        }

        std::string generateCode(std::unordered_set<uint64_t>& processedNodes) override {
            if (output)
                return output->generateCode(processedNodes) += "\n";

            return std::format("{} {} = {};\n", TypeTraits<T>::codeName, codeVar(), TypeTraits<T>::toInitCode(data()));
        }

        std::string codeVar() override {
            if (output)
                return output->codeVar(); // Link to output variable field

            return std::format("var{}", IHandle::id());
        }
    private:
        OutputHandle<T>* output = nullptr;
        uint64_t lastSeenOutputVersion = 0;
    };
}

#endif //INPUTHANDLE_H
