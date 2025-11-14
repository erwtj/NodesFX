#ifndef SINNODE_H
#define SINNODE_H

#include "../../NodeRegistry.h"
#include "../../../generator/INode.h"
#include "../../../generator/InputHandle.h"

using namespace generator;

namespace nodes {
    class SinNode final : public INode {
    public:
        explicit SinNode();
        ~SinNode() override = default;

        [[nodiscard]] const char* name() const override { return "Sin"; }
        static inline NodeRegistrar<SinNode> registrar{"Math/Trigonometry", "Sin"};

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<float>> input = std::make_shared<InputHandle<float>>("In", 0.0f);
        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("Out", this, 0.0f);
    };

}



#endif //SINNODE_H
