#ifndef ADDNODE_H
#define ADDNODE_H
#include <string>

#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/InputHandle.h"

using namespace generator;

namespace nodes {
    class AddNode final : public INode {
    public:
        explicit AddNode();
        ~AddNode() override = default;

        [[nodiscard]] const char* name() const override { return "Add"; }
        static inline NodeRegistrar<AddNode> registrar{"Math", "Add"};

        void setA(const float a) { inputA.setData(a); }
        void setB(const float b) { inputB.setData(b); }
        [[nodiscard]] float getResult() const { return output.data(); }

        InputHandle<float>* getInputAHandle() { return &inputA; }
        InputHandle<float>* getInputBHandle() { return &inputB; }
        OutputHandle<float>* getOutputHandle() { return &output; }

    protected:
        void processInternal() override;

    private:
        InputHandle<float> inputA{"A", 0.0f};
        InputHandle<float> inputB{"B", 0.0f};

        OutputHandle<float> output{"C", *this, 0.0f};
    };

}



#endif //ADDNODE_H
