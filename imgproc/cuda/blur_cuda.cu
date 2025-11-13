#include <cuda_runtime.h>
#include <cstdio>
#include <iostream>

extern "C"
__global__ void BlurKernel(int width, int height, float radius, const float* input, float* output)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    if ((x < width) && (y < height))
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
                    const int index = ((sampleY * width) + sampleX) * 4;
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

// TODO: This malloc / memcpy logic should be moved out of blur_cuda and should be generalised for performance reasons
void blur_cuda(const float* h_input, int width, int height, float radius, float* h_output)
{
    float *d_input, *d_output;
    size_t size = width * height * 4 * sizeof(float);

    cudaMalloc(&d_input, size);
    cudaMalloc(&d_output, size);

    cudaMemcpy(d_input, h_input, size, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks((width + threads.x - 1) / threads.x,
                (height + threads.y - 1) / threads.y);

    BlurKernel<<<blocks, threads>>>(width, height, radius, d_input, d_output);
    cudaDeviceSynchronize();

    cudaMemcpy(h_output, d_output, size, cudaMemcpyDeviceToHost);

    cudaFree(d_input);
    cudaFree(d_output);
}
