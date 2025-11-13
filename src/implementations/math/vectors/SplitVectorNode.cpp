#include "SplitVectorNode.h"

#include "inja.hpp"

template class nodes::SplitVectorNode<Vec<2>, 2>;
template class nodes::SplitVectorNode<Vec<3>, 3>;
template class nodes::SplitVectorNode<Vec<4>, 4>;

template <typename VecType, size_t N>
nodes::SplitVectorNode<VecType, N>::SplitVectorNode() {
    _inputHandles.push_back(input);

    static const char* names[] = {"X", "Y", "Z", "W"};
    for (size_t i = 0; i < N; ++i) {
        outputs[i] = std::make_shared<OutputHandle<float>>(names[i], this, 0.0f);
        _outputHandles.push_back(outputs[i]);
    }
}

template <typename VecType, size_t N>
void nodes::SplitVectorNode<VecType, N>::processInternal() {
    const VecType vec = input->data();
    for (size_t i = 0; i < N; ++i)
        outputs[i]->setData(vec.data[i]);
}

template <typename VecType, size_t N>
std::string nodes::SplitVectorNode<VecType, N>::generateCodeInternal() {
    std::string inVecVar = input->codeVar();

    std::string code = "// Split vec\n";
    for (int i = 0; i < N; i++) {
        std::string outVar = _outputHandles[i]->codeVar();
        code += std::format("float {} = {}.data[{}];\n", outVar, inVecVar, i);
    }

    return code;
}