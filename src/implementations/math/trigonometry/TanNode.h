#ifndef TANNODE_H
#define TANNODE_H

#include "../../NodeRegistry.h"
#include "../../../generator/nodes/INode.h"
#include "../../../generator/nodes/InputHandle.h"

using namespace generator;

namespace nodes {
    class TanNode final : public INode {
    public:
        explicit TanNode();
        ~TanNode() override = default;

        [[nodiscard]] const char* name() const override { return "Tan"; }
        static inline NodeRegistrar<TanNode> registrar{"Math/Trigonometry", "Tan"};
        std::string generateCode(std::unordered_set<uint64_t> processedNodes) override {return "";}

    protected:
        void processInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> input = std::make_shared<InputHandle<float>>("In", 0.0f);
        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("Out", this, 0.0f);
    };

}

#endif //TANNODE_H
