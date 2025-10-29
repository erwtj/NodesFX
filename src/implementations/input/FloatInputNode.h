#ifndef FLOATINPUTNODE_H
#define FLOATINPUTNODE_H

#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/InputHandle.h"

using namespace generator;

namespace nodes {
    class FloatInputNode final : public INode {
    public:
        explicit FloatInputNode();
        ~FloatInputNode() override = default;

        [[nodiscard]] const char* name() const override { return "Float"; }
        // This node does not register itself, since it will be accessed by the designer directly

        std::shared_ptr<OutputHandle<float>> output = std::make_shared<OutputHandle<float>>("", this, 0.0f);
    protected:
        void processInternal() override;
    };

}



#endif //FLOATINPUTNODE_H
