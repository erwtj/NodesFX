#ifndef TANNODE_H
#define TANNODE_H

#include "../../NodeRegistry.h"
#include "../../../generator/INode.h"
#include "../../../generator/InputHandle.h"

using namespace generator;

namespace nodes {
    class TanNode final : public INode {
    public:
        explicit TanNode();
        ~TanNode() override = default;

        [[nodiscard]] const char* name() const override { return "Tan"; }
        static inline NodeRegistrar<TanNode> registrar{"Math/Trigonometry", "Tan"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> input = std::make_shared<InputHandle<float>>("In", 0.0f);
        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("Out", this, 0.0f);
    };

}

#endif //TANNODE_H
