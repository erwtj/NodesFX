#ifndef VEC4_H
#define VEC4_H
#include <cmath>
#include <stdexcept>


struct Vec4 {
    float x;
    float y;
    float z;
    float w;

    Vec4()
        : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

    Vec4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {}

    float &operator[](size_t index) {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index out of range for Vec4");
        }
    }

    float operator[](size_t index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            case 3: return w;
            default: throw std::out_of_range("Index out of range for Vec4");
        }
    }

    float length() const {
        return std::sqrt(lengthSquared());
    }

    float lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }

    float dot(const Vec4 &other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    float distanceTo(const Vec4 &other) const {
        return (*this - other).length();
    }

    float distanceToSquared(const Vec4 &other) const {
        return (*this - other).lengthSquared();
    }

    Vec4 operator+(const Vec4 &other) const {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vec4 operator-(const Vec4 &other) const {
        return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Vec4 operator*(float scalar) const {
        return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    static Vec4 zero() {
        return Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    static Vec4 one() {
        return Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
};



#endif //VEC4_H
