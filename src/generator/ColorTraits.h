#ifndef COLORTRAITS_H
#define COLORTRAITS_H

#include "imgui.h"
#include <string>

#include "../util/TexData.h"
#include "../util/Vec4.h"

namespace generator {
    template<typename T>
    struct ColorTraits {
        // default color (gray)
        static ImU32 color() {
            return ImGui::ColorConvertFloat4ToU32(ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        }
    };

    // Specializations for common types
    template<>
    struct ColorTraits<int> {
        static ImU32 color() {
            return ImGui::ColorConvertFloat4ToU32(ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
        }
    };

    template<>
    struct ColorTraits<float> {
        static ImU32 color() {
            return ImGui::ColorConvertFloat4ToU32(ImVec4(0.3f, 0.9f, 0.3f, 1.0f));
        }
    };

    template<>
    struct ColorTraits<std::string> {
        static ImU32 color() {
            return ImGui::ColorConvertFloat4ToU32(ImVec4(0.3f, 0.5f, 0.9f, 1.0f));
        }
    };

    template<>
    struct ColorTraits<bool> {
        static ImU32 color() {
            return ImGui::ColorConvertFloat4ToU32(ImVec4(0.9f, 0.8f, 0.3f, 1.0f));
        }
    };

    template<>
    struct ColorTraits<Vec4> {
        static ImU32 color() {
            return ImGui::ColorConvertFloat4ToU32(ImVec4(0.5f, 0.3f, 0.9f, 1.0f));
        }
    };

    template<>
    struct ColorTraits<TexData> {
        static ImU32 color() {
            return IM_COL32(252, 186, 3, 255);
        }
    };
}

#endif //COLORTRAITS_H
