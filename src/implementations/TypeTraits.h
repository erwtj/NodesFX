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
        static std::string toCode(T t) {
            return "nullptr";
        }
    };

    template<>
    struct TypeTraits<int> {
        static Color color() {
            return {0.9f, 0.3f, 0.3f, 1.0f};
        }
        static std::string toCode(const int i) {
            return std::to_string(i);
        }
    };

    template<>
    struct TypeTraits<float> {
        static Color color() {
            return {0.3f, 0.9f, 0.3f, 1.0f};
        }
        static std::string toCode(const float f) {
            return std::to_string(f) += "f";
        }
    };

    template<>
    struct TypeTraits<std::string> {
        static Color color() {
            return {0.3f, 0.5f, 0.9f, 1.0f};
        }
        static std::string toCode(const std::string& str) {
            return std::format("\"{}\"", str);
        }
    };

    template<>
    struct TypeTraits<bool> {
        static Color color() {
            return {0.9f, 0.8f, 0.3f, 1.0f};
        }
        static std::string toCode(const bool b) {
            return b ? "true" : "false";
        }
    };

    template<size_t N>
    struct TypeTraits<Vec<N>> {
        static Color color() {
            return {0.5f, 0.3f, 0.9f, 1.0f};
        }
        static std::string toCode(const Vec<N>& vec) {
            std::string bracedList = "{";
            for (int i = 0; i < N - 1; i++) {
                bracedList += std::to_string(vec[i]) += ", ";
            }
            bracedList += std::to_string(vec[N - 1]) += "}";
            return std::format("Vec<{}>{}", N, bracedList);
        }
    };

    template<>
    struct TypeTraits<TexData> {
        static Color color() {
            return {0.988f, 0.73f, 0.01f, 1.0f};
        }
        static std::string toCode(const TexData& tex) {
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
        static std::string toCode(const Color& col) {
            return std::format("Color{{{}, {}, {}, {}}}", col.r, col.g, col.b, col.a);
        }
    };

}

#endif
