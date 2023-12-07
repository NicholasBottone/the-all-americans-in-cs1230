#include "raytracer/raytracer.h"

/**
 * Extra credit.
 * Code from filter project to offer antialiasing.
 * FilterBlur at bottom of file used in raytracer.
 */

enum KERNEL_CHANNEL {
    RED,
    GREEN,
    BLUE,
    NONE=-1,
};

struct Kernel1D {
    std::function<double(double, KERNEL_CHANNEL)> getWeight;
    double radius;
};

enum CONVOLVE_DIRECTION {
    HORIZONTAL,
    VERTICAL
};

RGBA getPixelWrapped(std::vector<RGBA> &data, int width, int height, int x, int y) {
    int newX = (x < 0) ? x + width  : x % width;
    int newY = (y < 0) ? y + height : y % height;
    return data[width * newY + newX];
}

std::uint8_t floatToUint8(float x) {
    x = std::min(255.f, x);
    return round(x * 255.f);
}

std::vector<RGBA> convolve1D(std::vector<RGBA> data, int width, int height, Kernel1D kernel, CONVOLVE_DIRECTION direction) {
    // need to assign then set, since the direction could be either way
    std::vector<RGBA> result;
    result.assign(width*height, RGBA{0, 0, 0, 255});

    // get the order of the for loop, based on the bound
    int outerBound = direction == CONVOLVE_DIRECTION::HORIZONTAL ? height : width;
    int innerBound = direction == CONVOLVE_DIRECTION::HORIZONTAL ? width : height;

    for (int i = 0; i < outerBound; i++) {
        for (int j = 0; j < innerBound; j++) {
            float redAcc = 0.f, greenAcc = 0.f, blueAcc = 0.f;
            for (int k = -kernel.radius; k <= kernel.radius; k++) {
                // get the weight for each channel, at this kernel index
                double rWeight = kernel.getWeight(k, KERNEL_CHANNEL::RED);
                double gWeight = kernel.getWeight(k, KERNEL_CHANNEL::GREEN);
                double bWeight = kernel.getWeight(k, KERNEL_CHANNEL::BLUE);

                // determine the pixel location on the canvas
                int pixelX = direction == CONVOLVE_DIRECTION::HORIZONTAL ? j + k : i;
                int pixelY = direction == CONVOLVE_DIRECTION::HORIZONTAL ? i : j + k;

                // get the pixel to compute this inner index of convolution.
                //  if out of bounds, get the wrapped
                RGBA pixel;
                if (pixelX < 0 || pixelX >= width || pixelY < 0 || pixelY >= height)
                    pixel = getPixelWrapped(data, width, height, pixelX, pixelY);
                else
                    pixel = data.at(width * pixelY + pixelX);

                // sum the weights on each channel
                redAcc += rWeight * pixel.r/255.f;
                greenAcc += gWeight * pixel.g/255.f;
                blueAcc += bWeight * pixel.b/255.f;
            }

            // get location then set the pixel into the result
            int pixelOnCanvas = direction == CONVOLVE_DIRECTION::HORIZONTAL ? width * i + j : width * j + i;
            result[pixelOnCanvas] = RGBA{floatToUint8(redAcc), floatToUint8(greenAcc), floatToUint8(blueAcc), 255};
        }
    }

    return result;
}

double triangleFilter(double x, double a) {
    double radius;
    if (a < 1) {
        radius = 1/a;
    } else {
        radius = 1;
    }

    if (x < -radius || x > radius)
        return 0;

    return (1 - std::fabs(x)/radius) / radius;
}

void RayTracer::filterBlur(RGBA *imageData, int width, int height, float blurRadius) {
    // make triangle filter
    // note: 1/blurRadius for the "radius" of the filter will normalize the area under it to 1
    Kernel1D triangleKernel;
    triangleKernel.radius = blurRadius;
    triangleKernel.getWeight = [blurRadius](double x, int c) { return triangleFilter(x, 1/blurRadius); };

    std::vector<RGBA> data{};
    for (int i = 0; i < width*height; i++) {
        data.push_back(imageData[i]);
    }

    std::vector<RGBA> res = convolve1D(data, width, height, triangleKernel, HORIZONTAL);
    res = convolve1D(res, width,height, triangleKernel, VERTICAL);

    for (int i = 0; i < res.size(); i++) {
        imageData[i] = res[i];
    }
}
