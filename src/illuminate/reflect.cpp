//
// Created by Michael Foiani on 11/4/23.
//

#include "raytracer/raytracer.h"

// helper that reflects vectors
glm::vec3 reflectVector(
        glm::vec3 incidentDir,
        glm::vec3 normal)
{
    return incidentDir - 2.f * glm::dot(incidentDir, normal) * normal;
}

glm::vec4 RayTracer::reflect(
        glm::vec3 intersectionWorld,
        glm::vec3 normalWorld,
        glm::vec3 incidentDir,
        const RenderShapeData &shape,
        const RayTraceScene &scene,
        int depth)
{
    auto material = shape.primitive.material;
    // check if the material is reflective
    if (material.cReflective == glm::vec4(0.f))
    {
        return glm::vec4(0.f);
    }
    auto reflectedDir = reflectVector(incidentDir, normalWorld);

    // shoot a ray from the intersection point in the reflected direction
    auto reflectColors = getPixelFromRay(glm::vec4(intersectionWorld + .001f * reflectedDir, 1.f), glm::vec4(reflectedDir, 0.f), scene, depth + 1);
    return scene.getGlobalData().ks * material.cReflective * reflectColors;
}

// EXTRA CREDIT -> refracting

// TRUE REFRACTING
// get the reflection coefficient from fresnel's equations
bool REAL_REFRACTING = false;
float fresnels(
        float currentMediumIor,
        float otherMediumIor,
        float cosAngleIncident,
        float cosAngleTransmitted)
{
    float rPerp = (currentMediumIor * cosAngleIncident - otherMediumIor * cosAngleTransmitted) /
                  (currentMediumIor * cosAngleIncident + otherMediumIor * cosAngleTransmitted);
    rPerp *= rPerp;
    float rPara = (otherMediumIor * cosAngleIncident - currentMediumIor * cosAngleTransmitted) /
                  (otherMediumIor * cosAngleIncident + currentMediumIor * cosAngleTransmitted);
    rPara *= rPara;
    return (rPerp + rPara) / 2.f;
}

// Your refracting
glm::vec4 RayTracer::refract(
        glm::vec3  intersectionWorld,
        glm::vec3  normalWorld,
        glm::vec3  incidentDir,
        const RenderShapeData& shape,
        const RayTraceScene &scene,
        int depth
)
{
    auto material = shape.primitive.material;
    // check if the material is transparent
    if (material.cTransparent == glm::vec4(0.f))
    {
        return glm::vec4(0.f);
    }

    // apply snells law to find the sin of refracted angle (squared)
    incidentDir = glm::normalize(incidentDir);
    float cosAngleIncident = glm::dot(incidentDir, normalWorld);
    float currentMediumIor = mediumIor;
    float otherMediumIor = material.ior;

    if (cosAngleIncident < 0)
    {
        // outside the object
        cosAngleIncident = -cosAngleIncident;
    }
    else
    {
        // inside the object, invert the normal and swap the Iors
        normalWorld = -normalWorld;
        std::swap(currentMediumIor, otherMediumIor);
    }

    float iorRatio = currentMediumIor / otherMediumIor;
    float sinAngleTransmittedSquared = iorRatio * iorRatio * (1 - cosAngleIncident * cosAngleIncident);
    if (sinAngleTransmittedSquared > 1.f) // total internal reflection, not considered
    {
        return glm::vec4(0.f);
    }

    auto cosAngleTransmitted = glm::sqrt(1 - sinAngleTransmittedSquared);

    // compute refracted ray according to snell's law
    auto refractedDir = glm::normalize(
            incidentDir * iorRatio
            + (iorRatio * cosAngleIncident - cosAngleTransmitted) * normalWorld);

    // send a ray in the refracted direction to get the colors
    auto refractedColors = getPixelFromRay(
            glm::vec4(intersectionWorld + .001f * refractedDir, 1.f),
            glm::vec4(refractedDir, 0.f),
            scene,
            depth + 1);

    float fresnel = fresnels(currentMediumIor, otherMediumIor, cosAngleIncident, cosAngleTransmitted);
    auto color = scene.getGlobalData().kt * material.cTransparent * refractedColors * (1 - fresnel);
    return color;
}