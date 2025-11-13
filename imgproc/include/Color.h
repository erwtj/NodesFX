#ifndef COLOR_H
#define COLOR_H
#include <stdexcept>

struct Color {
    float r;
    float g;
    float b;
    float a;

    Color()
        : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}

    Color(float r, float g, float b, float a)
        : r(r), g(g), b(b), a(a) {}

    float &operator[](size_t index) {
        switch (index) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        default: throw std::out_of_range("Index out of range for Color");
        }
    }

    float operator[](size_t index) const {
        switch (index) {
        case 0: return r;
        case 1: return g;
        case 2: return b;
        case 3: return a;
        default: throw std::out_of_range("Index out of range for Color");
        }
    }

    float luminance() const {
        return 0.2126f * r + 0.7152f * g + 0.0722f * b;
    }

    Color operator+(const Color &other) const {
        return Color(r + other.r, g + other.g, b + other.b, a + other.a);
    }

    Color operator-(const Color &other) const {
        return Color(r - other.r, g - other.g, b - other.b, a - other.a);
    }

    Color operator*(float scalar) const {
        return Color(r * scalar, g * scalar, b * scalar, a * scalar);
    }

    static Color zero() {
        return Color(0.0f, 0.0f, 0.0f, 0.0f);
    }

    static Color one() {
        return Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    inline friend std::ostream & operator<<(std::ostream & os, const Color & v) {
        return os << v.r << ", " << v.g << ", " << v.b << ", " << v.a;
    }

    explicit operator std::string() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }
};

#endif //COLOR_H
