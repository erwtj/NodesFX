#ifndef NODEREGISTRY_H
#define NODEREGISTRY_H
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include "../generator/nodes/INode.h"

using namespace generator;

class NodeRegistry {
public:
    using Factory = std::function<std::unique_ptr<INode>()>;

    struct Entry {
        std::string name;
        Factory create;
    };

    struct CategoryNode {
        std::unordered_map<std::string, std::unique_ptr<CategoryNode>> subcategories;
        std::vector<Entry> entries;
    };

    // Register a node at a hierarchical path like "Math/Noise/Perlin"
    static void registerNode(const std::string& categoryPath, const std::string& name, Factory factory) {
        std::lock_guard<std::mutex> lock(mutex_);
        CategoryNode* node = &root_;
        for (const auto& cat : splitPath(categoryPath)) {
            if (!node->subcategories.count(cat))
                node->subcategories[cat] = std::make_unique<CategoryNode>();
            node = node->subcategories[cat].get();
        }
        node->entries.push_back({ name, std::move(factory) });
    }

    // Return list of subcategories in a given category path
    static std::vector<std::string> subcategories(const std::string& categoryPath = "") {
        std::lock_guard<std::mutex> lock(mutex_);
        CategoryNode* node = findCategory(categoryPath);
        if (!node) return {};
        std::vector<std::string> result;
        for (auto& kv : node->subcategories)
            result.push_back(kv.first);
        return result;
    }

    // Return nodes (entries) within a given category path
    static std::vector<Entry> get(const std::string& categoryPath) {
        std::lock_guard<std::mutex> lock(mutex_);
        CategoryNode* node = findCategory(categoryPath);
        if (!node) return {};
        return node->entries;
    }

    // Create a node by category path and name
    static std::unique_ptr<INode> create(const std::string& categoryPath, const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex_);
        CategoryNode* node = findCategory(categoryPath);
        if (!node) return nullptr;
        for (auto& e : node->entries)
            if (e.name == name)
                return e.create();
        return nullptr;
    }

    // Return all category paths (for UI menus, etc.)
    static std::vector<std::string> allCategoryPaths() {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::string> result;
        collectPaths(&root_, "", result);
        return result;
    }

private:
    inline static CategoryNode root_;
    inline static std::mutex mutex_;

    static std::vector<std::string> splitPath(const std::string& path) {
        std::vector<std::string> parts;
        std::stringstream ss(path);
        std::string part;
        while (std::getline(ss, part, '/')) {
            if (!part.empty())
                parts.push_back(part);
        }
        return parts;
    }

    static CategoryNode* findCategory(const std::string& path) {
        if (path.empty()) return &root_;
        CategoryNode* node = &root_;
        for (const auto& part : splitPath(path)) {
            auto it = node->subcategories.find(part);
            if (it == node->subcategories.end()) return nullptr;
            node = it->second.get();
        }
        return node;
    }

    static void collectPaths(CategoryNode* node, const std::string& prefix, std::vector<std::string>& out) {
        for (auto& kv : node->subcategories) {
            std::string full = prefix.empty() ? kv.first : prefix + "/" + kv.first;
            out.push_back(full);
            collectPaths(kv.second.get(), full, out);
        }
    }
};

// Registration helper
template <typename T>
struct NodeRegistrar {
    NodeRegistrar(const std::string& categoryPath, const std::string& name) {
        NodeRegistry::registerNode(categoryPath, name, [] { return std::make_unique<T>(); });
    }
};

#endif // NODEREGISTRY_H
