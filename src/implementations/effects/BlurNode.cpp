#include "BlurNode.h"

using namespace nodes;

BlurNode::BlurNode() : INode() {
    _inputHandles.push_back(inputTex);
    _inputHandles.push_back(radius);

    _outputHandles.push_back(outputTex);
}

void BlurNode::processInternal() {
    const float* inputData = inputTex->data().getData();

    if (inputData == nullptr)
    {
        // No input texture, output empty texture
        outputTex->setData(TexData());
        return;
    }

    const int width = inputTex->data().getWidth();
    const int height = inputTex->data().getHeight();
    const float r = radius->data();

    auto* outputData = new float[width * height * 4];
    // Simple box blur
    int kernelSize = static_cast<int>(r) * 2 + 1;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float rSum = 0.0f, gSum = 0.0f, bSum = 0.0f, aSum = 0.0f;
            int count = 0;
            for (int ky = -static_cast<int>(r); ky <= static_cast<int>(r); ky++)
            {
                for (int kx = -static_cast<int>(r); kx <= static_cast<int>(r); kx++)
                {
                    const int sampleX = x + kx;
                    const int sampleY = y + ky;
                    if (sampleX >= 0 && sampleX < width && sampleY >= 0 &&
                        sampleY < height)
                    {
                        const int index = (sampleY * width + sampleX) * 4;
                        rSum += inputData[index + 0];
                        gSum += inputData[index + 1];
                        bSum += inputData[index + 2];
                        aSum += inputData[index + 3];
                        count++;
                    }
                }
            }
            const int outIndex = (y * width + x) * 4;
            outputData[outIndex + 0] = rSum / static_cast<float>(count);
            outputData[outIndex + 1] = gSum / static_cast<float>(count);
            outputData[outIndex + 2] = bSum / static_cast<float>(count);
            outputData[outIndex + 3] = aSum / static_cast<float>(count);
        }
    }

    outputTex->setData(TexData(outputData, width, height));
}
