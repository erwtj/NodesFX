#ifndef INPUTMAKER_H
#define INPUTMAKER_H

#include "imgui.h"
#include <Vec.h>
#include <Color.h>

template<typename T>
struct InputTraits;

template<>
struct InputTraits<int> {
    static constexpr float width = 25;
    static constexpr int defaultValue() { return 0; }
    static bool drawWidget(const char* label, int& value) {
        return ImGui::InputInt(label, &value, 0, 0, ImGuiInputTextFlags_None);
    }
};

template<>
struct InputTraits<float> {
    static constexpr float width = 25;
    static constexpr float defaultValue() { return 0.0f; }
    static bool drawWidget(const char* label, float& value) {
        return ImGui::InputFloat(label, &value);
    }
};

template<>
struct InputTraits<bool> {
    static constexpr float width = 15;
    static constexpr bool defaultValue() { return false; }
    static bool drawWidget(const char* label, bool& value) {
        return ImGui::Checkbox(label, &value);
    }
};

template<>
struct InputTraits<Vec<4>> {
    static constexpr float width = 180;
    static Vec<4> defaultValue() { return Vec<4>(); }
    static bool drawWidget(const char* label, Vec<4>& value) {
        return ImGui::InputFloat4(label, &value.x);
    }
};

template<>
struct InputTraits<Vec<3>> {
    static constexpr float width = 150;
    static Vec<3> defaultValue() { return Vec<3>(); }
    static bool drawWidget(const char* label, Vec<3>& value) {
        return ImGui::InputFloat3(label, &value.x);
    }
};

template<>
struct InputTraits<Vec<2>> {
    static constexpr float width = 120;
    static Vec<2> defaultValue() { return Vec<2>(); }
    static bool drawWidget(const char* label, Vec<2>& value) {
        return ImGui::InputFloat2(label, &value.x);
    }
};

template<>
struct InputTraits<Color> {
    static constexpr float width = 150;
    static Color defaultValue() { return {}; }
    static bool drawWidget(const char* label, Color& value) {
        return ImGui::ColorEdit4(label, &value.r);
    }
};


#endif //INPUTMAKER_H
