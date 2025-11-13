#include "VectorLengthNode.h"

template class nodes::VectorLengthNode<Vec<2>, 2>;
template class nodes::VectorLengthNode<Vec<3>, 3>;
template class nodes::VectorLengthNode<Vec<4>, 4>;

template <typename VecType, size_t N>
nodes::VectorLengthNode<VecType, N>::VectorLengthNode() {
    _inputHandles.emplace_back(input);
    _outputHandles.emplace_back(output);
}

template <typename VecType, size_t N>
void nodes::VectorLengthNode<VecType, N>::processInternal() {
    Vec<N> vec = input->data();
    output->setData(vec.length());
}

template <typename VecType, size_t N>
std::string nodes::VectorLengthNode<VecType, N>::generateCodeInternal() {
    std::string inVecVar = input->codeVar();
    std::string outLengthVar = output->codeVar();

    return std::format("// Vector length\nfloat {} = {}.length();", outLengthVar, inVecVar);
}