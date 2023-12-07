#include "raytracer/raytracer.h"

/**
 * Extra credit -> Super Sampling
 */

const float SUPERSAMPLE_DISTANCE_FROM_CENTER = .25f; // note: max of .5f, unless overlapping with other pixels
bool SUPER_SAMPLE = false;
bool ADAPTIVE_SUPER_SAMPLING = false;

RGBA RayTracer::superSample(
        glm::vec4 eyeCamera,
        glm::vec4 pixelDirCamera,
        const RayTraceScene &scene) {
    // get the color value at value between center and four corners
    float x_delta = SUPERSAMPLE_DISTANCE_FROM_CENTER / (scene.width());
    float y_delta = SUPERSAMPLE_DISTANCE_FROM_CENTER / (scene.height());
    // TL == TOP LEFT
    // BR = BOTTOM RIGHT, not Battle Royale :)
    glm::vec4 pixelTL = getPixelFromRay(
            eyeCamera,
            glm::vec4(pixelDirCamera.x - x_delta, pixelDirCamera.y - y_delta, pixelDirCamera.z, 0.f),
            scene);
    glm::vec4 pixelTR = getPixelFromRay(
            eyeCamera,
            glm::vec4(pixelDirCamera.x + x_delta, pixelDirCamera.y - y_delta, pixelDirCamera.z, 0.f),
            scene);
    glm::vec4 pixelBL = getPixelFromRay(
            eyeCamera,
            glm::vec4(pixelDirCamera.x - x_delta, pixelDirCamera.y + y_delta, pixelDirCamera.z, 0.f),
            scene);
    glm::vec4 pixelBR = getPixelFromRay(
            eyeCamera,
            glm::vec4(pixelDirCamera.x + x_delta, pixelDirCamera.y + y_delta, pixelDirCamera.z, 0.f),
            scene);

    if (!ADAPTIVE_SUPER_SAMPLING) {
        return toRGBA((pixelTL + pixelTR + pixelBL + pixelBR) / 4.f);
    }

    // ADAPTIVE SUPER SAMPLING
    // make the region from the center of pixel smaller until we hit something
    RGBA nohit = {0, 0, 0, 0}; // just here to say that a is 0 if no hit...
    float TRAVERSE_DISTANCE = .025f;
    float num_pixels = 4.f;
    if (pixelTL.a == 0) {
        num_pixels--;
        float smallerDist = SUPERSAMPLE_DISTANCE_FROM_CENTER - TRAVERSE_DISTANCE;
        while (smallerDist < TRAVERSE_DISTANCE) {
            float x_delta = smallerDist / (scene.width());
            float y_delta = smallerDist / (scene.height());
            pixelTL = getPixelFromRay(
                    eyeCamera,
                    glm::vec4(pixelDirCamera.x - x_delta, pixelDirCamera.y - y_delta, pixelDirCamera.z, 0.f),
                    scene);
            if (pixelTL.a != 0) {
                num_pixels++;
                break;
            }
            smallerDist -= TRAVERSE_DISTANCE;
        }
    }
    if (pixelTR.a == 0) {
        num_pixels--;
        float smallerDist = SUPERSAMPLE_DISTANCE_FROM_CENTER - TRAVERSE_DISTANCE;
        while (smallerDist < TRAVERSE_DISTANCE) {
            float x_delta = smallerDist / (scene.width());
            float y_delta = smallerDist / (scene.height());
            pixelTR = getPixelFromRay(
                    eyeCamera,
                    glm::vec4(pixelDirCamera.x - x_delta, pixelDirCamera.y - y_delta, pixelDirCamera.z, 0.f),
                    scene);
            if (pixelTR.a != 0) {
                num_pixels += 1;
                break;
            }
            smallerDist -= TRAVERSE_DISTANCE;
        }
    }
    if (pixelBL.a == 0) {
        num_pixels--;
        float smallerDist = SUPERSAMPLE_DISTANCE_FROM_CENTER - TRAVERSE_DISTANCE;
        while (smallerDist < TRAVERSE_DISTANCE) {
            float x_delta = smallerDist / (scene.width());
            float y_delta = smallerDist / (scene.height());
            pixelBL = getPixelFromRay(
                    eyeCamera,
                    glm::vec4(pixelDirCamera.x - x_delta, pixelDirCamera.y - y_delta, pixelDirCamera.z, 0.f),
                    scene);
            if (pixelBL.a != 0) {
                num_pixels += 1;
                break;
            }
            smallerDist -= TRAVERSE_DISTANCE;
        }
    }
    if (pixelBR.a == 0) {
        num_pixels--;
        float smallerDist = SUPERSAMPLE_DISTANCE_FROM_CENTER - TRAVERSE_DISTANCE;
        while (smallerDist < TRAVERSE_DISTANCE) {
            float x_delta = smallerDist / (scene.width());
            float y_delta = smallerDist / (scene.height());
            pixelBR = getPixelFromRay(
                    eyeCamera,
                    glm::vec4(pixelDirCamera.x - x_delta, pixelDirCamera.y - y_delta, pixelDirCamera.z, 0.f),
                    scene);
            if (pixelBR.a != 0) {
                num_pixels += 1;
                break;
            }
            smallerDist -= TRAVERSE_DISTANCE;
        }
    }

    if (num_pixels == 0.f) {
        return nohit;
    }
    return toRGBA((pixelTL + pixelTR + pixelBL + pixelBR) / num_pixels);
}
