#ifndef COLORTRAITSIMPL_H
#define COLORTRAITSIMPL_H

#include <format>

#include "Color.h"
#include "Vec.h"
#include "TexData.h"

namespace generator {
    template<typename T>
    struct TypeTraits {
        static Color color() {
            return {0.6f, 0.6f, 0.6f, 1.0f};
        }

        static constexpr const char* displayName = "Unknown";
        static constexpr const char* codeName = "auto";

        static std::string toInitCode(T t) {
            return "nullptr";
        }
    };

    template<>
    struct TypeTraits<int> {
        static Color color() {
            return {0.9f, 0.3f, 0.3f, 1.0f};
        }

        static constexpr const char* displayName = "Int";
        static constexpr const char* codeName = "int";

        static std::string toInitCode(const int i) {
            return std::to_string(i);
        }
    };

    template<>
    struct TypeTraits<float> {
        static Color color() {
            return {0.3f, 0.9f, 0.3f, 1.0f};
        }

        static constexpr const char* displayName = "Float";
        static constexpr const char* codeName = "float";

        static std::string toInitCode(const float f) {
            std::ostringstream oss;
            oss << f;
            return oss.str();
        }
    };

    template<>
    struct TypeTraits<std::string> {
        static Color color() {
            return {0.3f, 0.5f, 0.9f, 1.0f};
        }

        static constexpr const char* displayName = "String";
        static constexpr const char* codeName = "std::string";

        static std::string toInitCode(const std::string& str) {
            return std::format("\"{}\"", str);
        }
    };

    template<>
    struct TypeTraits<bool> {
        static Color color() {
            return {0.9f, 0.8f, 0.3f, 1.0f};
        }

        static constexpr const char* displayName = "Boolean";
        static constexpr const char* codeName = "bool";

        static std::string toInitCode(const bool b) {
            return b ? "true" : "false";
        }
    };

    template<>
    struct TypeTraits<Vec2> {
        static Color color() {
            return {0.5f, 0.3f, 0.9f, 1.0f};
        }

        static constexpr const char* displayName = "Vec2";
        static constexpr const char* codeName = "Vec2";

        static std::string toInitCode(const Vec2& vec) {
            return std::format("Vec2{{{}, {}}}", vec.x, vec.y);
        }
    };

    template<>
    struct TypeTraits<Vec3> {
        static Color color() {
            return {0.5f, 0.3f, 0.9f, 1.0f};
        }

        static constexpr const char* displayName = "Vec3";
        static constexpr const char* codeName = "Vec3";

        static std::string toInitCode(const Vec3& vec) {
            return std::format("Vec3{{{}, {}, {}}}", vec.x, vec.y, vec.z);
        }
    };

    template<>
    struct TypeTraits<Vec4> {
        static Color color() {
            return {0.5f, 0.3f, 0.9f, 1.0f};
        }

        static constexpr const char* displayName = "Vec4";
        static constexpr const char* codeName = "Vec4";

        static std::string toInitCode(const Vec4& vec) {
            return std::format("Vec4{{{}, {}, {}, {}}}", vec.x, vec.y, vec.z, vec.w);
        }
    };


    template<>
    struct TypeTraits<TexData> {
        static Color color() {
            return {0.988f, 0.73f, 0.01f, 1.0f};
        }

        static constexpr const char* displayName = "Texture";
        static constexpr const char* codeName = "TexData";

        static std::string toInitCode(const TexData& tex) {
            const int width = tex.getWidth();
            const int height = tex.getHeight();
            return std::format("TexData{{new float[{} * {} * 4], {}, {}}}", width, height, width, height);
        }
    };

    template<>
    struct TypeTraits<Color> {
        static Color color() {
            return {1.0f, 0.0f, 1.0f, 1.0f};
        }

        static constexpr const char* displayName = "Color";
        static constexpr const char* codeName = "Color";

        static std::string toInitCode(const Color& col) {
            return std::format("Color{{{}, {}, {}, {}}}", col.r, col.g, col.b, col.a);
        }
    };

}

#endif
