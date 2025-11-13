#ifndef VECTORLENGTHNODE_H
#define VECTORLENGTHNODE_H

#include "../../NodeRegistry.h"
#include "../../../generator/nodes/INode.h"
#include "../../../generator/nodes/InputHandle.h"
#include "../../../generator/nodes/OutputHandle.h"
#include "../../../../imgproc/include/Vec.h"

namespace nodes {
    template <typename VecType, size_t N>
    class VectorLengthNode final : public INode {
    public:
        explicit VectorLengthNode();
        ~VectorLengthNode() override = default;

        [[nodiscard]] const char* name() const override {
            if constexpr (N == 2) return "Length vec2";
            else if constexpr (N == 3) return "Length vec3";
            else if constexpr (N == 4) return "Length vec4";
            else return "Length vecN";
        }

        static inline NodeRegistrar<VectorLengthNode<VecType, N>> registrar{
            "Math/Vectors",
            N == 2 ? "Length vec2" :
            N == 3 ? "Length vec3" :
            N == 4 ? "Length vec4" : "Length vecN"
        };

    protected:
        void processInternal() override;
        std::string generateCodeInternal() override;

    private:
        std::shared_ptr<InputHandle<VecType>> input =
            std::make_shared<InputHandle<VecType>>("Vector", VecType{});

        std::shared_ptr<OutputHandle<float>> output =
            std::make_shared<OutputHandle<float>>("Length", this, 0.0f);
    };
}



#endif //VECTORLENGTHNODE_H
