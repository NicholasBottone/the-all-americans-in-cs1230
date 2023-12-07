#include <QList>
#include <QtConcurrent>
#include <iostream>
#include "raytracer.h"
#include "raytracescene.h"

//struct Ray {
//    glm::vec3 p;
//    glm::vec3 d;
//};

RayTracer::RayTracer(const Config &config) : m_config(config) {}

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    if(m_config.enableParallelism)
    {
        renderParallel(imageData, scene);
        return;
    }

    // naive rendering
    Camera camera = scene.getCamera();
    float cameraDepth = 1.f;

    float viewplaneHeight = 2.f*cameraDepth*std::tan(camera.getHeightAngle() / 2.f);
    float viewplaneWidth = cameraDepth*viewplaneHeight*((float)scene.width()/(float)scene.height());

    for (int imageRow = 0; imageRow < scene.height(); imageRow++) {
        for (int imageCol = 0; imageCol < scene.width(); imageCol++) {
            float xCameraSpace = viewplaneWidth *
                                 (-.5f + (imageCol + .5f) / scene.width());
            float yCameraSpace = viewplaneHeight *
                                 (-.5f + (imageRow + .5f) / scene.height());

            glm::vec4 pixelDirCamera{xCameraSpace, -yCameraSpace, -cameraDepth, 0.f}; //w=0 for dir
            glm::vec4 eyeCamera{0.f, 0.f, 0.f, 1.f}; // w=1.f for point

            // convert to world space
            glm::vec4 pWorld = camera.getInverseViewMatrix() * eyeCamera;
            glm::vec4 dWorld = glm::normalize(camera.getInverseViewMatrix() * pixelDirCamera);

            // cast ray!
            glm::vec4 pixel = getPixelFromRay(pWorld, dWorld, scene);
            imageData[imageRow * scene.width() + imageCol] = toRGBA(pixel);
        }
    }
}


glm::vec4 RayTracer::getPixelFromRay(
    glm::vec4 pWorld,
    glm::vec4 dWorld,
    const RayTraceScene &scene,
    int depth)
{
    if (depth > m_config.maxRecursiveDepth)
    {
        return glm::vec4(0.f);
    }

    // variables from computing the intersection
    glm::vec4 closestIntersectionObj;
    glm::vec4 closestIntersectionWorld;
    RenderShapeData intersectedShape;

    if (m_config.enableAcceleration)
    {
        float tWorld = traverseBVH(pWorld, dWorld, intersectedShape, scene.m_bvh);
        if (tWorld == FINF)
        {
            return glm::vec4(0.f);
        }
        closestIntersectionWorld = pWorld + tWorld * dWorld;
        closestIntersectionObj = intersectedShape.inverseCTM * closestIntersectionWorld;
    }
    else
    {
        float minDist = FINF;
        // shoot a ray at each shape
        for (const RenderShapeData &shape : scene.getShapes()) {
            glm::vec4 pObject = shape.inverseCTM * pWorld;
            glm::vec4 dObject = glm::normalize(shape.inverseCTM * dWorld);

            glm::vec4 newIntersectionObj = findIntersection(pObject, dObject, shape);
            if (newIntersectionObj.w == 0) // no hit
            {
                continue;
            }

            auto newIntersectionWorld = shape.ctm * newIntersectionObj;
            float newDist = glm::distance(newIntersectionWorld, pWorld);
            if (
                    newDist < minDist // closer intersection
                    && !floatEquals(newDist, 0) // and not a self intersection
                    )
            {
                minDist = newDist;

                intersectedShape = shape;
                closestIntersectionObj = newIntersectionObj;
                closestIntersectionWorld = newIntersectionWorld;
            }
        }

        if (minDist == FINF) // no hit
        {
            return glm::vec4(0.f);
        }
    }

    glm::vec3 normalObject = getNormal(closestIntersectionObj, intersectedShape, scene);
    glm::vec3 normalWorld =
            (
                    glm::inverse(glm::transpose(intersectedShape.ctm))
                    * glm::vec4(normalObject, 0.f)
            ).xyz();

    return illuminatePixel(closestIntersectionWorld, normalWorld, -dWorld, intersectedShape, scene, depth);
}

// EXTRA CREDIT -> depth of field
glm::vec4 RayTracer::secondaryRays(glm::vec4 pWorld, glm::vec4 dWorld, RayTraceScene &scene)
{
    auto inv = scene.getCamera().getInverseViewMatrix();
    float focalLength = scene.getCamera().getFocalLength();
    float aperture = scene.getCamera().getAperture();

    glm::vec4 illumination(0.f);
    glm::vec4 focalPoint = pWorld + focalLength * dWorld;

    int TIMES = 500;
    for (int i = 0; i < TIMES; i++) {
        // generate a random number from -aperature to aperature
        float rand1 = ((float) rand() / (float) RAND_MAX) * aperture;
        rand1 *= (rand() % 2 == 0) ? 1 : -1;
        // generate another number also inside the aperature lens
        float rand2 = ((float) rand() / (float) RAND_MAX) * std::sqrt(aperture - rand1*rand1);
        rand2 *= (rand() % 2 == 0) ? 1 : -1;
        glm::vec4 randEye = (rand() % 2 == 0) ? glm::vec4(rand1, rand2, 0.f, 1.f) : glm::vec4(rand2, rand1, 0.f, 1.f);
        // convert this random point to world space
        glm::vec4 eyeWorld = inv * randEye;

        // make the ray
        glm::vec4 randomDir = glm::vec4(glm::normalize(focalPoint.xyz() - eyeWorld.xyz()), 0.f);

        illumination += getPixelFromRay(eyeWorld, randomDir, scene, 0);
    }

    return illumination / (float) TIMES;
}