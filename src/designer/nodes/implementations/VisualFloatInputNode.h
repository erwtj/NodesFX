#ifndef VISUALFLOATINPUTNODE_H
#define VISUALFLOATINPUTNODE_H
#include "../VisualNode.h"
#include "../../../generator/OutputHandle.h"


class VisualFloatInputNode : public VisualNode {
public:
    explicit VisualFloatInputNode();
    ~VisualFloatInputNode() override = default;

    void draw() override;

private:
    std::shared_ptr<generator::OutputHandle<float>> _outputHandle;
};



#endif //VISUALFLOATINPUTNODE_H
