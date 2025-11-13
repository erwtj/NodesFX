#ifndef SUBTRACTNODE_H
#define SUBTRACTNODE_H

#include "../../NodeRegistry.h"
#include "../../../generator/nodes/INode.h"
#include "../../../generator/nodes/InputHandle.h"

using namespace generator;

namespace nodes {
    class SubtractNode final : public INode {
    public:
        explicit SubtractNode();
        ~SubtractNode() override = default;

        [[nodiscard]] const char* name() const override { return "Subtract"; }
        static inline NodeRegistrar<SubtractNode> registrar{"Math/Operators", "Subtract"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> inputA = std::make_shared<InputHandle<float>>("A", 0.0f);
        std::shared_ptr<InputHandle<float>> inputB = std::make_shared<InputHandle<float>>("B", 0.0f);

        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("C", this, 0.0f);
    };

}




#endif //SUBTRACTNODE_H
