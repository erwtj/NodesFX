#ifndef POWERNODE_H
#define POWERNODE_H

#include "../../NodeRegistry.h"
#include "../../../generator/INode.h"
#include "../../../generator/InputHandle.h"

using namespace generator;

namespace nodes {
    class PowerNode final : public INode {
    public:
        explicit PowerNode();
        ~PowerNode() override = default;

        [[nodiscard]] const char* name() const override { return "Power"; }
        static inline NodeRegistrar<PowerNode> registrar{"Math/Operators", "Power"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> inputA = std::make_shared<InputHandle<float>>("A", 0.0f);
        std::shared_ptr<InputHandle<float>> inputB = std::make_shared<InputHandle<float>>("B", 0.0f);

        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("C", this, 0.0f);
    };

}


#endif //POWERNODE_H
