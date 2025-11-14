#ifndef VEC_H
#define VEC_H
#include <cmath>
#include <sstream>

template <size_t N>
struct Vec {
    float data[N];

    Vec() : data{} {}

    Vec(std::initializer_list<float> init) : data{} {
        size_t i = 0;
        for (float f : init) {
            if (i < N) data[i++] = f;
        }
    }

    float& operator[](size_t i) { return data[i]; }
    const float& operator[](size_t i) const { return data[i]; }

    Vec<N> operator+(const Vec<N>& other) const {
        Vec<N> result;
        for (size_t i = 0; i < N; ++i) result[i] = data[i] + other[i];
        return result;
    }

    Vec<N> operator-(const Vec<N>& other) const {
        Vec<N> result;
        for (size_t i = 0; i < N; ++i) result[i] = data[i] - other[i];
        return result;
    }

    Vec<N> operator*(float scalar) const {
        Vec<N> result;
        for (size_t i = 0; i < N; ++i) result[i] = data[i] * scalar;
        return result;
    }

    float dot(const Vec<N>& other) const {
        float result = 0.0f;
        for (size_t i = 0; i < N; ++i) result += data[i] * other[i];
        return result;
    }

    [[nodiscard]] float length() const {
        return std::sqrt(dot(*this));
    }
};

template <size_t N>
inline std::ostream& operator<<(std::ostream& os, const Vec<N>& v) {
    for (size_t i = 0; i < N; ++i) {
        os << v.data[i];
        if (i < N - 1) os << ", ";
    }
    return os;
}

// Specializations for easy access
template <>
struct Vec<2> {
    union { struct { float x, y; }; float data[2]; };

    Vec(float x = 0, float y = 0) : x(x), y(y) {}

    Vec<2> operator+(const Vec<2>& o) const { return {x + o.x, y + o.y}; }
    Vec<2> operator-(const Vec<2>& o) const { return {x - o.x, y - o.y}; }
    Vec<2> operator*(float s) const { return {x * s, y * s}; }
    float dot(const Vec<2>& o) const { return x * o.x + y * o.y; }
    float length() const { return std::sqrt(dot(*this)); }

    float &operator[](size_t i) {
        switch (i) {
        case 0: return x;
        case 1: return y;
        default: throw std::out_of_range("Index out of range for Vec2");
        }
    }
    const float &operator[](size_t i) const {
        switch (i) {
        case 0: return x;
        case 1: return y;
        default: throw std::out_of_range("Index out of range for Vec2");
        }
    }
};

template <>
struct Vec<3> {
    union { struct { float x, y, z; }; float data[3]; };

    Vec(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vec<3> operator+(const Vec<3>& o) const { return {x + o.x, y + o.y, z + o.z}; }
    Vec<3> operator-(const Vec<3>& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vec<3> operator*(float s) const { return {x * s, y * s, z * s}; }
    float dot(const Vec<3>& o) const { return x * o.x + y * o.y + z * o.z; }
    Vec<3> cross(const Vec<3>& o) const {
        return {y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x};
    }
    float length() const { return std::sqrt(dot(*this)); }

    float &operator[](size_t i) {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Index out of range for Vec3");
        }
    }
    const float &operator[](size_t i) const {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Index out of range for Vec3");
        }
    }
};

template <>
struct Vec<4> {
    union { struct { float x, y, z, w; }; float data[4]; };

    Vec(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}

    Vec<4> operator+(const Vec<4>& o) const { return {x + o.x, y + o.y, z + o.z, w + o.w}; }
    Vec<4> operator-(const Vec<4>& o) const { return {x - o.x, y - o.y, z - o.z, w - o.w}; }
    Vec<4> operator*(float s) const { return {x * s, y * s, z * s, w * s}; }
    float dot(const Vec<4>& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }
    float length() const { return std::sqrt(dot(*this)); }

    float &operator[](size_t i) {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: throw std::out_of_range("Index out of range for Vec4");
        }
    }
    const float &operator[](size_t i) const {
        switch (i) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: throw std::out_of_range("Index out of range for Vec4");
        }
    }
};

using Vec2 = Vec<2>;
using Vec3 = Vec<3>;
using Vec4 = Vec<4>;

#endif //VEC_H
