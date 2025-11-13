#ifndef SOLIDCOLORNODE_H
#define SOLIDCOLORNODE_H

#include "../NodeRegistry.h"
#include "../../generator/nodes/INode.h"
#include "../../generator/nodes/InputHandle.h"
#include "../../../imgproc/include/Color.h"

namespace nodes {
    class SolidColorNode : public INode {
    public:
        explicit SolidColorNode();
        ~SolidColorNode() override = default;

        [[nodiscard]] const char* name() const override { return "Solid Color"; }
        static inline NodeRegistrar<SolidColorNode> registrar{"Generate", "Solid Color"};
        std::string generateCode(std::unordered_set<uint64_t> processedNodes) override {return "";}

    protected:
        void processInternal() override;

    private:
        std::shared_ptr<InputHandle<Color>> color = std::make_shared<InputHandle<Color>>("Color", Color(1.0f, 0.0f, 0.0f, 1.0f));
        std::shared_ptr<InputHandle<Vec2>> size = std::make_shared<InputHandle<Vec2>>("Size", Vec2(100, 100));

        std::shared_ptr<OutputHandle<TexData>> output = std::make_shared<OutputHandle<TexData>>("Solid", this, TexData());
    };
}

#endif //SOLIDCOLORNODE_H
