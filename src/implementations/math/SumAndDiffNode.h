#ifndef SUMANDDIFFNODE_H
#define SUMANDDIFFNODE_H

#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/InputHandle.h"
#include "../../util/Vec4.h"

namespace nodes {
    class SumAndDiffNode final : public INode {
    public:
        explicit SumAndDiffNode();
        ~SumAndDiffNode() override = default;

        [[nodiscard]] const char* name() const override { return "sum/diff vec4"; }
        static inline NodeRegistrar<SumAndDiffNode> registrar{"Math", "sum/diff vec4"};

    protected:
        void processInternal() override;

    private:
        std::shared_ptr<InputHandle<Vec4>> inputA = std::make_shared<InputHandle<Vec4>>("Vector 1", Vec4{0.0f, 0.0f, 0.0f, 0.0f});
        std::shared_ptr<InputHandle<Vec4>> inputB = std::make_shared<InputHandle<Vec4>>("Vector 2", Vec4{0.0f, 0.0f, 0.0f, 0.0f});

        std::shared_ptr<OutputHandle<Vec4>> outputSum = std::make_shared<OutputHandle<Vec4>>("Sum", this, Vec4());
        std::shared_ptr<OutputHandle<Vec4>> outputDiff = std::make_shared<OutputHandle<Vec4>>("Diff", this, Vec4());
    };
}



#endif //SUMANDDIFFNODE_H
