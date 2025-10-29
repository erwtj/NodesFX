#ifndef SAMPLETEXTURENODE_H
#define SAMPLETEXTURENODE_H

#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/OutputHandle.h"

using namespace generator;

namespace nodes {
    class SampleTextureNode final : public INode {
    public:
        explicit SampleTextureNode();
        ~SampleTextureNode() override = default;

        [[nodiscard]] const char* name() const override { return "Sample Texture"; }
        static inline NodeRegistrar<SampleTextureNode> registrar{"IO", "Sample Texture"};

    protected:
        void processInternal() override;

    private:
        std::shared_ptr<OutputHandle<TexData>> output = std::make_shared<OutputHandle<TexData>>("Sample", this, TexData());
    };
} // namespace nodes


#endif // SAMPLETEXTURENODE_H
