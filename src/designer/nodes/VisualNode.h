#ifndef VISUALNODE_H
#define VISUALNODE_H
#include "IVisualNode.h"
#include "imgui_node_editor.h"
#include "VisualHandle.h"
#include "../../generator/nodes/INode.h"
#include "../../implementations/NodeRegistry.h"
#include "../rendering/GLTextureHandle.h"


// Node wrapper
class VisualNode : public IVisualNode {
public:
    VisualNode(VisualNode&&) noexcept = default;
    VisualNode& operator=(VisualNode&&) noexcept = delete;
    VisualNode(const VisualNode&) = delete;
    VisualNode& operator=(const VisualNode&) = delete;

    explicit VisualNode(std::unique_ptr<INode> node);
    ~VisualNode() override { delete texHandle; }

    void draw() override;

    static std::unique_ptr<VisualNode> createFromRegistryEntry(const NodeRegistry::Entry& entry);

    [[nodiscard]] const char* getName() const override { return _node->name(); }

    [[nodiscard]] const std::vector<std::shared_ptr<VisualHandle>>& getInputHandles() const override { return _inputHandles; }
    [[nodiscard]] const std::vector<std::shared_ptr<VisualHandle>>& getOutputHandles() const override { return _outputHandles; }

    [[nodiscard]] std::string generateCode() override;

protected:
    uint64_t texVersion = 0;
    std::shared_ptr<IDataHandle<TexData>> texDataHandle = nullptr;
    ITextureHandle* texHandle = nullptr;

    std::unique_ptr<INode> _node;

    std::vector<std::shared_ptr<VisualHandle>> _inputHandles = {};
    std::vector<std::shared_ptr<VisualHandle>> _outputHandles = {};
};



#endif //VISUALNODE_H
