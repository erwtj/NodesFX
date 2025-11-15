#ifndef TEXDATA_H
#define TEXDATA_H

#include <memory>
#include <sstream>

class TexData {
public:
    TexData() = default;

    TexData(float* data, int width, int height)
        : impl(std::make_shared<Impl>()) {
        impl->setData(data, width, height);
    }

    // Copyable and assignable (shared_ptr handles shared ownership)
    TexData(const TexData&) = default;
    TexData& operator=(const TexData&) = default;

    // Accessors
    [[nodiscard]] float* getData() const { return impl ? impl->data.get() : nullptr; }
    [[nodiscard]] int getWidth() const { return impl ? impl->width : 0; }
    [[nodiscard]] int getHeight() const { return impl ? impl->height : 0; }

    // Modifiers
    void setData(float* newData, int width, int height) {
        if (!impl) impl = std::make_shared<Impl>();
        impl->setData(newData, width, height);
    }

    inline friend std::ostream& operator<<(std::ostream& os, const TexData& v) {
        return os << "width: " << v.getWidth() << ", height: " << v.getHeight();
    }

    explicit operator std::string() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }

private:
    struct Impl {
        std::shared_ptr<float[]> data;
        int width = 0, height = 0;

        void setData(float* newData, int w, int h) {
            data.reset(newData, std::default_delete<float[]>());
            width = w;
            height = h;
        }

        Impl() = default;
    };

    std::shared_ptr<Impl> impl;
};

#endif //TEXDATA_H
