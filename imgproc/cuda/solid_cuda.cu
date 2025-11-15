#include <cuda_runtime.h>
#include <cstdio>
#include <iostream>

#include "../include/imgproc.h"

extern "C"
__global__ void SolidKernel(const int width, const int height, const float r, const float g, const float b, const float a, float* output)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    if ((x < width) && (y < height))
    {
        const int outIndex = (y * width + x) * 4;
        output[outIndex + 0] = r;
        output[outIndex + 1] = g;
        output[outIndex + 2] = b;
        output[outIndex + 3] = a;
    }
}

float* solid_cuda(const int width, const int height, const Color& color)
{
    float *d_output;
    size_t size = width * height * 4 * sizeof(float);

    cudaMalloc(&d_output, size);

    dim3 threads(16, 16);
    dim3 blocks((width + threads.x - 1) / threads.x,
                (height + threads.y - 1) / threads.y);

    SolidKernel<<<blocks, threads>>>(width, height, color.r, color.g, color.b, color.a, d_output);
    cudaDeviceSynchronize();

    float* h_output = new float[width * height * 4];
    cudaMemcpy(h_output, d_output, size, cudaMemcpyDeviceToHost);
    cudaFree(d_output);

    return h_output;
}
