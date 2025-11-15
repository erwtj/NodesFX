#ifndef PROCESSORS_H
#define PROCESSORS_H

#include "Color.h"

// cpu
void blur_cpu(const float* input, int width, int height, float radius, float* output);
float* solid_cpu(int width, int height, const Color& color);
float* sample_gradient_cpu(int width, int height);

// cuda
void blur_cuda(const float* input, int width, int height, float radius, float* output);
float* solid_cuda(int width, int height, const Color& color);
float* sample_gradient_cuda(int width, int height);

#endif //PROCESSORS_H
