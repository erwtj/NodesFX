#ifndef BOOLNODE_H
#define BOOLNODE_H

#include "../InputHandle.h"
#include "../Node.h"

class BoolNode final : public Node {
public:
    explicit BoolNode() : Node() {
        _inputHandles.push_back(&input);
        _outputHandles.push_back(&output);
    }
    ~BoolNode() override = default;

    void setInput(const bool val) { input.setData(val); }
    [[nodiscard]] bool getResult() const { return output.data(); }

    InputHandle<bool>* getInputHandle() { return &input; }
    OutputHandle<bool>* getOutputHandle() { return &output; }

protected:
    void processInternal() override {
        output.setData(!input.data());
    }

private:
    InputHandle<bool> input{*this, false};

    OutputHandle<bool> output{*this, true};
};

#endif //BOOLNODE_H
