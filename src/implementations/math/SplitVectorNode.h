#ifndef SPLITVECTORNODE_H
#define SPLITVECTORNODE_H

#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/InputHandle.h"
#include "../../math/Vec4.h"

using namespace generator;

namespace nodes {
    class SplitVectorNode final : public INode {
    public:
        explicit SplitVectorNode();
        ~SplitVectorNode() override = default;

        [[nodiscard]] const char* name() const override { return "Split vec4"; }
        static inline NodeRegistrar<SplitVectorNode> registrar{"Math", "Split vec4"};

    protected:
        void processInternal() override;

    private:
        std::shared_ptr<InputHandle<Vec4>> input = std::make_shared<InputHandle<Vec4>>("Vector", Vec4{0.0f, 0.0f, 0.0f, 0.0f});

        std::shared_ptr<OutputHandle<float>> outputX = std::make_shared<OutputHandle<float>>("X", *this, 0.0f);
        std::shared_ptr<OutputHandle<float>> outputY = std::make_shared<OutputHandle<float>>("Y", *this, 0.0f);
        std::shared_ptr<OutputHandle<float>> outputZ = std::make_shared<OutputHandle<float>>("Z", *this, 0.0f);
        std::shared_ptr<OutputHandle<float>> outputW = std::make_shared<OutputHandle<float>>("W", *this, 0.0f);
    };

}



#endif //SPLITVECTORNODE_H
