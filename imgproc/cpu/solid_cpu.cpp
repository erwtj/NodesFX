#include "../include/imgproc.h"

float* solid_cpu(const int width, const int height, const Color& color)
{
    float* output = new float[width * height * 4];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const int outIndex = (y * width + x) * 4;
            output[outIndex + 0] = color.r;
            output[outIndex + 1] = color.g;
            output[outIndex + 2] = color.b;
            output[outIndex + 3] = color.a;
        }
    }
    return output;
}
