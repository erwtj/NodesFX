#include "../include/imgproc.h"

float* sample_gradient_cpu(const int width, const int height)
{
    float _width = static_cast<float>(width);
    float _height = static_cast<float>(height);

    float* out = new float[width * height * 4];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const float r = static_cast<float>(x) / _width;
            const float g = static_cast<float>(y) / _height;
            constexpr float b = 0.5f;
            constexpr float a = 1.0f;

            const int index = (y * width + x) * 4;
            out[index + 0] = r;
            out[index + 1] = g;
            out[index + 2] = b;
            out[index + 3] = a;
        }
    }

    return out;
}
