#ifndef INPUTNODE_H
#define INPUTNODE_H

#include "InputTraits.h"
#include "../../../generator/OutputHandle.h"
#include "../../../generator/INode.h"

using namespace generator;

// Not in nodes namespace since this is frontend dependent, not part of the core generator

template<typename T>
class InputNode final : public INode {
public:
    explicit InputNode() : INode() { _outputHandles.push_back(output); }
    ~InputNode() override = default;

    [[nodiscard]] const char* name() const override { return TypeTraits<T>::displayName; }

    std::shared_ptr<OutputHandle<T>> output = std::make_shared<OutputHandle<T>>("", this, InputTraits<T>::defaultValue());
protected:
    void processInternal() override {};
    std::string generateCodeInternal() override {
        return std::format("{} {} = {};",
            TypeTraits<T>::codeName,
            output->codeVar(),
            TypeTraits<T>::toInitCode(output->data())
        );
    }
};

#endif //INPUTNODE_H
