#ifndef PROCESSORS_H
#define PROCESSORS_H

// Blur
void blur_cpu(const float* input, int width, int height, float radius, float* output);
void blur_cuda(const float* input, int width, int height, float radius, float* output);

#endif //PROCESSORS_H
