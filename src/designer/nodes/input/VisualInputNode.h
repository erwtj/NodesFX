#ifndef VISUALINPUTNODE_H
#define VISUALINPUTNODE_H

#include "InputTraits.h"
#include "InputNode.h"
#include "InputNodeRegistry.h"
#include "../VisualNode.h"
#include "../../../generator/nodes/OutputHandle.h"
#include "../../../implementations/math/AddNode.h"

template<typename T>
class VisualInputNode final : public VisualNode {
public:
    VisualInputNode() : VisualNode(std::make_unique<InputNode<T>>()), label("##" + std::to_string(static_cast<uint64_t>(_nodeId))) {
        _outputHandle = std::dynamic_pointer_cast<OutputHandle<T>>(_node->_outputHandles[0]);
    }

    static inline InputNodeRegistrar<VisualInputNode<T>> registrar{InputTraits<T>::name};

    void draw() override {
        namespace ed = ax::NodeEditor;
        ed::BeginNode(_nodeId);

        ImGui::Text("%s", InputTraits<T>::name);

        float handleSizeOffset = CIRCLE_RADIUS * 6;
        float totalWidth = std::max(ImGui::CalcTextSize(_outputHandle->name()).x, InputTraits<T>::width) + handleSizeOffset + 20.0f;
        totalWidth = std::max(totalWidth, 100.0f);

        T value = _outputHandle->data();
        ImGui::SetNextItemWidth(totalWidth - handleSizeOffset);

        if (InputTraits<T>::drawWidget(label.c_str(), value)) {
            _outputHandle->setData(value);
        }

        ImGui::SameLine();
        _outputHandles[0]->draw(totalWidth);

        ed::EndNode();
    }

private:
    std::shared_ptr<OutputHandle<T>> _outputHandle;
    const std::string label;
};

#endif //VISUALINPUTNODE_H
