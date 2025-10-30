#ifndef SPLITVECTORNODE_H
#define SPLITVECTORNODE_H

#include <array>
#include "../NodeRegistry.h"
#include "../../generator/INode.h"
#include "../../generator/InputHandle.h"
#include "../../generator/OutputHandle.h"
#include "../../util/Vec.h"

using namespace generator;

namespace nodes {

    template <typename VecType, size_t N>
    class SplitVectorNode final : public INode {
    public:
        explicit SplitVectorNode() {
            _inputHandles.push_back(input);

            static const char* names[] = {"X", "Y", "Z", "W"};
            for (size_t i = 0; i < N; ++i) {
                outputs[i] = std::make_shared<OutputHandle<float>>(names[i], this, 0.0f);
                _outputHandles.push_back(outputs[i]);
            }
        }

        ~SplitVectorNode() override = default;

        [[nodiscard]] const char* name() const override {
            if constexpr (N == 2) return "Split vec2";
            else if constexpr (N == 3) return "Split vec3";
            else if constexpr (N == 4) return "Split vec4";
            else return "Split vecN";
        }

        static inline NodeRegistrar<SplitVectorNode<VecType, N>> registrar{
            "Math",
            N == 2 ? "Split vec2" :
            N == 3 ? "Split vec3" :
            N == 4 ? "Split vec4" : "Split vecN"
        };

    protected:
        void processInternal() override {
            const VecType vec = input->data();
            for (size_t i = 0; i < N; ++i)
                outputs[i]->setData(vec.data[i]);
        }

    private:
        std::shared_ptr<InputHandle<VecType>> input =
            std::make_shared<InputHandle<VecType>>("Vector", VecType{});

        std::array<std::shared_ptr<OutputHandle<float>>, N> outputs;
    };

    // Concrete aliases
    using SplitVector2Node = SplitVectorNode<Vec<2>, 2>;
    using SplitVector3Node = SplitVectorNode<Vec<3>, 3>;
    using SplitVector4Node = SplitVectorNode<Vec<4>, 4>;

} // namespace nodes

#endif // SPLITVECTORNODE_H
