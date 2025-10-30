#ifndef INPUTNODEREGISTRY_H
#define INPUTNODEREGISTRY_H

#include <functional>
#include <memory>
#include <mutex>
#include <string>

#include "../IVisualNode.h"

class InputNodeRegistry {
public:
    using Factory = std::function<std::unique_ptr<IVisualNode>()>;

    struct Entry {
        std::string name;
        Factory create;
    };

    static void registerNode(const std::string& name, Factory factory) {
        std::lock_guard<std::mutex> lock(mutex_);
        registry.push_back({ name, std::move(factory) });
    }

    static const std::vector<Entry>& entries() {
        std::lock_guard<std::mutex> lock(mutex_);
        return registry;
    }

    static std::unique_ptr<IVisualNode> create(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [_name, create] : registry)
            if (_name == name)
                return create();
        return nullptr;
    }

private:
    inline static std::vector<Entry> registry;
    inline static std::mutex mutex_;
};

template <typename T>
struct InputNodeRegistrar {
    explicit InputNodeRegistrar(const std::string& name) {
        InputNodeRegistry::registerNode(name, [] { return std::make_unique<T>(); });
    }
};

#endif //INPUTNODEREGISTRY_H
