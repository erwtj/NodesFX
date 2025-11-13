#ifndef SAMPLETEXTURENODE_H
#define SAMPLETEXTURENODE_H

#include "../NodeRegistry.h"
#include "../../generator/nodes/INode.h"
#include "../../generator/nodes/OutputHandle.h"

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
