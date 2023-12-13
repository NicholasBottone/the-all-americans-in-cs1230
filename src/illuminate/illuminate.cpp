#include <iostream>
#include "raytracer/raytracer.h"

glm::vec4  RayTracer::illuminationFromDirectionalLight(
        const SceneLightData &light,
        glm::vec4 intersectionWorld,
        glm::vec4 normalWorld,
        glm::vec4 directionToCamera,
        const RenderShapeData &shape,
        const RayTraceScene &scene
)
{
    // define direction and distance of directional light
    auto directionFromIntersectionToLight = - light.dir;
    directionFromIntersectionToLight = glm::normalize(directionFromIntersectionToLight);
    float distanceToLight = FINF;   // directional light infinitely far away

    float kd = scene.getGlobalData().kd;
    float ks = scene.getGlobalData().ks;
    auto material = shape.primitive.material;

    glm::vec4 illumination(0, 0, 0, 1.f);

    // calculate diffuse term
    auto dotDiffuse = glm::dot(normalWorld, directionFromIntersectionToLight);
    if (dotDiffuse < 0) {
        return illumination;
    }
    // add the diffuse
    auto diffuse = (kd * material.cDiffuse);
    illumination += light.color * dotDiffuse * diffuse;

    // if the obj has no specular, return
    if (material.cSpecular == glm::vec4(0.f)) {
        return illumination;
    }

    // get the light reflection vector
    auto reflectedDirOverNormal = glm::normalize(glm::reflect(directionFromIntersectionToLight, normalWorld));

    // calculate specular term
    auto dotSpecular = glm::dot(reflectedDirOverNormal, -directionToCamera);
    if (dotSpecular > 0) {
        auto toPow = std::pow(dotSpecular, material.shininess);
        illumination += light.color * toPow * (ks * material.cSpecular);
    }

    return illumination;
}



// Calculates the RGBA of a pixel from intersection infomation and globally-defined coefficients
glm::vec4 RayTracer::illuminatePixel(
        glm::vec4  intersectionWorld,
        glm::vec4  normalWorld,
        glm::vec4  directionToCamera,
        const RenderShapeData& shape,
        const RayTraceScene &scene,
        int depth)
{
    // Normalizing directions
    normalWorld           = glm::normalize(normalWorld);
    directionToCamera = glm::normalize(directionToCamera);

    // to be summed then returned
    glm::vec4 illumination(0, 0, 0, 1.f);

    // add the ambient term
    float ka = scene.getGlobalData().ka;
    illumination += ka*shape.primitive.material.cAmbient;

    for (const SceneLightData &light : scene.getLights()) {
        switch (light.type) {
            case LightType::LIGHT_POINT:
                continue;
            case LightType::LIGHT_DIRECTIONAL:
                illumination +=
                        illuminationFromDirectionalLight(light, intersectionWorld, normalWorld, directionToCamera, shape, scene);
                continue;
            case LightType::LIGHT_SPOT:
                continue;
            case LightType::LIGHT_AREA:
                continue;
            default:
                continue;
        }
    }

     auto incidentDir = -directionToCamera;
    // recursive raytracing for the reflection and refraction (see reflect.cpp)
     illumination += refract(intersectionWorld, normalWorld, incidentDir, shape, scene, depth + 1);
     illumination += reflect(intersectionWorld, normalWorld, incidentDir, shape, scene, depth + 1);

    return illumination;
}