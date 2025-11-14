#include <cuda_runtime.h>
#include <cstdio>
#include <iostream>

#include "../include/imgproc.h"

extern "C"
__global__ void SampleGradientKernel(const int width, const int height, float* output)
{
    const int x = blockIdx.x * blockDim.x + threadIdx.x;
    const int y = blockIdx.y * blockDim.y + threadIdx.y;

    float _width = static_cast<float>(width);
    float _height = static_cast<float>(height);

    if ((x < width) && (y < height))
    {
        const float r = static_cast<float>(x) / _width;
        const float g = static_cast<float>(y) / _height;
        constexpr float b = 0.5f;
        constexpr float a = 1.0f;

        const int index = (y * width + x) * 4;
        output[index + 0] = r;
        output[index + 1] = g;
        output[index + 2] = b;
        output[index + 3] = a;
    }
}

// TODO: Maybe figure out some way to instantly copy this into TexData
// We are already doing memory copies and stuff and we know the new width and height so why not?
float* sample_gradient_cuda(const int width, const int height)
{
    float *d_output;
    size_t size = width * height * 4 * sizeof(float);

    cudaMalloc(&d_output, size);

    dim3 threads(16, 16);
    dim3 blocks((width + threads.x - 1) / threads.x,
                (height + threads.y - 1) / threads.y);

    SampleGradientKernel<<<blocks, threads>>>(width, height, d_output);
    cudaDeviceSynchronize();

    float* h_output = new float[width * height * 4];
    cudaMemcpy(h_output, d_output, size, cudaMemcpyDeviceToHost);
    cudaFree(d_output);

    return h_output;
}