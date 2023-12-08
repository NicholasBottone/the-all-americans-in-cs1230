#include "raytracer/raytracer.h"

bool RayTracer::isShadowed(
        glm::vec4 lightPosition,
        float distanceToLight,
        glm::vec4 directionFromIntersectionToLight,
        glm::vec4 intersectionWorld,
        const RayTraceScene &scene)
{
    // normalize direction
    directionFromIntersectionToLight = glm::normalize(directionFromIntersectionToLight);

     // acceleration causes "bad jaggies" so we disable it for now
    if (m_enableAcceleration)
    {
        RenderShapeData shapeData;
        auto pBias = intersectionWorld + .001f * directionFromIntersectionToLight;
        float t = traverseBVH(pBias, directionFromIntersectionToLight, shapeData, scene.m_bvh);
        return t != FINF;
    }

    for (const RenderShapeData &s: scene.getShapes()) {
        // convert this world ray to object space
        glm::vec4 dObject = glm::normalize(
                s.inverseCTM * directionFromIntersectionToLight);
        glm::vec4 pObject = s.inverseCTM * intersectionWorld;

        // see if there is an intersection
        glm::vec4 newIntersectionObj = findIntersection(pObject, dObject, s);

        if (newIntersectionObj.w == 1.f) // hit!
        {
            // check if the intersection is the same as the pObject
            if (floatEquals(glm::distance(newIntersectionObj, pObject), 0.f, 0.001f))
            {
                // don't consider self-intersections
                continue;
            }

            // check if this intersection is closer than the direction to the light
            auto newIntersectionWorld = s.ctm * newIntersectionObj;
            if (distanceToLight == FINF)
            {
                // if the light is infinitely far away light, then any non-self intersection is valid
                return true;
            }

            float newDist = glm::distance(newIntersectionWorld, lightPosition);
            if (newDist < distanceToLight - 0.001f)
            {
                // an object in front of the camera is the way -> shadow
                return true;
            }
        }
    }

    return false;
}
