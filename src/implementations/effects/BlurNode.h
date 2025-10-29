#ifndef BLURNODE_H
#define BLURNODE_H
#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/InputHandle.h"

using namespace generator;
namespace nodes {
    class BlurNode final : public INode {
    public:
        explicit BlurNode();
        ~BlurNode() override = default;

        [[nodiscard]] const char* name() const override { return "Blur"; }
        static inline NodeRegistrar<BlurNode> registrar{"Effects", "Blur"};

    protected:
        void processInternal() override;

    private:
        std::shared_ptr<InputHandle<TexData>> inputTex = std::make_shared<InputHandle<TexData>>("Texture", TexData());
        std::shared_ptr<InputHandle<float>> radius = std::make_shared<InputHandle<float>>("Radius", 50.0f);

        std::shared_ptr<OutputHandle<TexData>> outputTex = std::make_shared<OutputHandle<TexData>>("Blurred", this, TexData());
    };

}

#endif //BLURNODE_H
