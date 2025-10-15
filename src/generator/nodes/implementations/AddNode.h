#ifndef ADDNODE_H
#define ADDNODE_H
#include "../InputHandle.h"
#include "../Node.h"


class AddNode final : public Node {
public:
    explicit AddNode();
    ~AddNode() override = default;

    void setA(const float a) { inputA.setData(a); }
    void setB(const float b) { inputB.setData(b); }
    [[nodiscard]] float getResult() const { return output.data(); }

    InputHandle<float>* getInputAHandle() { return &inputA; }
    InputHandle<float>* getInputBHandle() { return &inputB; }
    OutputHandle<float>* getOutputHandle() { return &output; }

protected:
    void processInternal() override;

private:
    InputHandle<float> inputA{0.0f};
    InputHandle<float> inputB{0.0f};

    OutputHandle<float> output{this, 0.0f};
};



#endif //ADDNODE_H
