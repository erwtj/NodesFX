#ifndef NODEREGISTRY_H
#define NODEREGISTRY_H
#include <functional>
#include <memory>
#include <mutex>
#include <string>

#include "../generator/INode.h"

using namespace generator;

class NodeRegistry {
public:
    using Factory = std::function<std::unique_ptr<INode>()>;

    struct Entry {
        std::string name;
        Factory create;
    };

    static void registerNode(const std::string& category, const std::string& name, Factory factory) {
        std::lock_guard<std::mutex> lock(mutex_);
        registry_[category].push_back({ name, std::move(factory) });
    }

    static std::vector<std::string> categories() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::string> keys;
        for (auto& kv : registry_) keys.push_back(kv.first);
        return keys;
    }

    static const std::vector<Entry>& get(const std::string& category) {
        std::lock_guard<std::mutex> lock(mutex_);
        return registry_[category];
    }

    static std::unique_ptr<INode> create(const std::string& category, const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = registry_.find(category);
        if (it == registry_.end()) return nullptr;
        for (auto& e : it->second)
            if (e.name == name)
                return e.create();
        return nullptr;
    }

private:
    inline static std::unordered_map<std::string, std::vector<Entry>> registry_;
    inline static std::mutex mutex_;
};

template <typename T>
struct NodeRegistrar {
    NodeRegistrar(const std::string& category, const std::string& name) {
        NodeRegistry::registerNode(category, name, [] { return std::make_unique<T>(); });
    }
};

#endif //NODEREGISTRY_H
