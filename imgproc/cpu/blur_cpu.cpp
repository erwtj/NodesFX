#include "../include/imgproc.h"

void blur_cpu(const float* input, const int width, const int height, const float radius, float* output)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float rSum = 0.0f, gSum = 0.0f, bSum = 0.0f, aSum = 0.0f;
            int count = 0;
            for (int ky = -static_cast<int>(radius); ky <= static_cast<int>(radius); ky++)
            {
                for (int kx = -static_cast<int>(radius); kx <= static_cast<int>(radius); kx++)
                {
                    const int sampleX = x + kx;
                    const int sampleY = y + ky;
                    if (sampleX >= 0 && sampleX < width && sampleY >= 0 &&
                        sampleY < height)
                    {
                        const int index = (sampleY * width + sampleX) * 4;
                        rSum += input[index + 0];
                        gSum += input[index + 1];
                        bSum += input[index + 2];
                        aSum += input[index + 3];
                        count++;
                    }
                }
            }
            const int outIndex = (y * width + x) * 4;
            output[outIndex + 0] = rSum / static_cast<float>(count);
            output[outIndex + 1] = gSum / static_cast<float>(count);
            output[outIndex + 2] = bSum / static_cast<float>(count);
            output[outIndex + 3] = aSum / static_cast<float>(count);
        }
    }
}