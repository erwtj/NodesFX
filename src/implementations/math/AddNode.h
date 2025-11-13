#ifndef ADDNODE_H
#define ADDNODE_H

#include "../NodeRegistry.h"
#include "../../generator/nodes/INode.h"
#include "../../generator/nodes/InputHandle.h"

using namespace generator;

namespace nodes {
    class AddNode final : public INode {
    public:
        explicit AddNode();
        ~AddNode() override = default;

        [[nodiscard]] const char* name() const override { return "Add"; }
        static inline NodeRegistrar<AddNode> registrar{"Math", "Add"};
        std::string generateCode(std::unordered_set<uint64_t> processedNodes) override {return "";}

    protected:
        void processInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> inputA = std::make_shared<InputHandle<float>>("A", 0.0f);
        std::shared_ptr<InputHandle<float>> inputB = std::make_shared<InputHandle<float>>("B", 0.0f);

        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("C", this, 0.0f);
    };

}



#endif //ADDNODE_H
