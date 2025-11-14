#ifndef SAMPLETEXTURENODE_H
#define SAMPLETEXTURENODE_H

#include "TexData.h"
#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/OutputHandle.h"

using namespace generator;

namespace nodes {
    class SampleGradientTextureNode final : public INode {
    public:
        explicit SampleGradientTextureNode();
        ~SampleGradientTextureNode() override = default;

        [[nodiscard]] const char* name() const override { return "Sample Gradient Texture"; }
        static inline NodeRegistrar<SampleGradientTextureNode> registrar{"Generate", "Sample Gradient Texture"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<OutputHandle<TexData>> output = std::make_shared<OutputHandle<TexData>>("Sample", this, TexData());
    };
} // namespace nodes


#endif // SAMPLETEXTURENODE_H
