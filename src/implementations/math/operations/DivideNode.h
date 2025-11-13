#ifndef DIVIDENODE_H
#define DIVIDENODE_H


#include "../../NodeRegistry.h"
#include "../../../generator/nodes/INode.h"
#include "../../../generator/nodes/InputHandle.h"

using namespace generator;

namespace nodes {
    class DivideNode final : public INode {
    public:
        explicit DivideNode();
        ~DivideNode() override = default;

        [[nodiscard]] const char* name() const override { return "Divide"; }
        static inline NodeRegistrar<DivideNode> registrar{"Math/Operators", "Divide"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> inputA = std::make_shared<InputHandle<float>>("A", 0.0f);
        std::shared_ptr<InputHandle<float>> inputB = std::make_shared<InputHandle<float>>("B", 0.0f);

        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("C", this, 0.0f);
    };

}



#endif //DIVIDENODE_H
