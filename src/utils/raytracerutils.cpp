//
// Created by Michael Foiani on 11/4/23.
//

#include "raytracer/raytracer.h"

// Helper function to convert illumination to RGBA, applying some form of tone-mapping (e.g. clamping) in the process
RGBA RayTracer::toRGBA(const glm::vec4 &illumination) {
    // Task 1
    return RGBA
            {
                    (std::uint8_t) (255 * std::clamp(illumination.r, 0.f, 1.f)),
                    (std::uint8_t) (255 * std::clamp(illumination.g, 0.f, 1.f)),
                    (std::uint8_t) (255 * std::clamp(illumination.b, 0.f, 1.f)),
                    (std::uint8_t) (255 * std::clamp(illumination.b, 0.f, 1.f))
            };
}

bool RayTracer::floatEquals(float a, float b, float epsilon) {
    return std::abs(a - b) <= epsilon;
}