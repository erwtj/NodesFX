#ifndef COSNODE_H
#define COSNODE_H

#include "../../NodeRegistry.h"
#include "../../../generator/nodes/INode.h"
#include "../../../generator/nodes/InputHandle.h"

using namespace generator;

namespace nodes {
    class CosNode final : public INode {
    public:
        explicit CosNode();
        ~CosNode() override = default;

        [[nodiscard]] const char* name() const override { return "Cos"; }
        static inline NodeRegistrar<CosNode> registrar{"Math/Trigonometry", "Cos"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> input = std::make_shared<InputHandle<float>>("In", 0.0f);
        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("Out", this, 1.0f);
    };

}



#endif //COSNODE_H
