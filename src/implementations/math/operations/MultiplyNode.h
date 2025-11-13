#ifndef MULTIPLYNODE_H
#define MULTIPLYNODE_H


#include "../../NodeRegistry.h"
#include "../../../generator/nodes/INode.h"
#include "../../../generator/nodes/InputHandle.h"

using namespace generator;

namespace nodes {
    class MultiplyNode final : public INode {
    public:
        explicit MultiplyNode();
        ~MultiplyNode() override = default;

        [[nodiscard]] const char* name() const override { return "Multiply"; }
        static inline NodeRegistrar<MultiplyNode> registrar{"Math/Operators", "Multiply"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> inputA = std::make_shared<InputHandle<float>>("A", 0.0f);
        std::shared_ptr<InputHandle<float>> inputB = std::make_shared<InputHandle<float>>("B", 0.0f);

        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("C", this, 0.0f);
    };

}



#endif //MULTIPLYNODE_H
