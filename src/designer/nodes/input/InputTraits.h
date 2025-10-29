#ifndef INPUTMAKER_H
#define INPUTMAKER_H
#include "../../../generator/OutputHandle.h"

template<typename T>
struct InputTraits;

template<>
struct InputTraits<int> {
    static constexpr const char* name = "Integer";
    static constexpr int defaultValue() { return 0; }
    static bool drawWidget(const char* label, int& value) {
        return ImGui::InputInt(label, &value);
    }
};

template<>
struct InputTraits<float> {
    static constexpr const char* name = "Float";
    static constexpr float defaultValue() { return 0.0f; }
    static bool drawWidget(const char* label, float& value) {
        return ImGui::InputFloat(label, &value);
    }
};

template<>
struct InputTraits<bool> {
    static constexpr const char* name = "Boolean";
    static constexpr bool defaultValue() { return false; }
    static bool drawWidget(const char* label, bool& value) {
        return ImGui::Checkbox(label, &value);
    }
};

template<>
struct InputTraits<Vec4> {
    static constexpr const char* name = "Vec4";
    static Vec4 defaultValue() { return Vec4(); }
    static bool drawWidget(const char* label, Vec4& value) {
        return ImGui::InputFloat4(label, &value.x);
    }
};



#endif //INPUTMAKER_H
