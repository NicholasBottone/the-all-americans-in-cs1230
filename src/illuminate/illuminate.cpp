#include <iostream>
#include "raytracer/raytracer.h"

glm::vec4  RayTracer::illuminationFromPointLight(
        const SceneLightData &light,
        glm::vec4 intersectionWorld,
        glm::vec4 normalWorld,
        glm::vec4  directionToCamera,
        const RenderShapeData &shape,
        const RayTraceScene &scene
        )
{
    auto directionFromIntersectionToLight = light.pos - intersectionWorld;
    directionFromIntersectionToLight = glm::normalize(directionFromIntersectionToLight);

    // check if this light is blocked by an object
    auto distanceToLight = glm::distance(light.pos, intersectionWorld);
    bool isShadow = RayTracer::isShadowed(
            light.pos,
            distanceToLight,
            glm::vec4(directionFromIntersectionToLight),
            glm::vec4(intersectionWorld),
            scene);
    if (isShadow)
    {
        // if this is a shadow, then no light contribution
        return glm::vec4(0.f);
    }

    // calculate attenuation
    float c1 = light.function.x;
    float c2 = light.function.y;
    float c3 = light.function.z;
    float attenuation = std::min(1.f, 1.f / (c1 + distanceToLight * c2 + (distanceToLight * distanceToLight) * c3));

    return phong(
            light.color,
            attenuation,
            directionFromIntersectionToLight,
            directionToCamera,
            intersectionWorld,
            normalWorld,
            shape,
            scene);
}

glm::vec4  RayTracer::illuminationFromSpotLight(
        const SceneLightData &light,
        glm::vec4 intersectionWorld,
        glm::vec4 normalWorld,
        glm::vec4 directionToCamera,
        const RenderShapeData &shape,
        const RayTraceScene &scene
)
{
    auto distance = glm::distance(light.pos, intersectionWorld);

    // calculate the angle from the shape to the spot light
    auto directionFromIntersectionToLight = glm::normalize(light.pos - intersectionWorld);

    // calculate intensity, based on angle. apply falloff if necessary
    auto lightDirection = glm::normalize(light.dir);
    // invert the direction of the intersection to light for dot product to work correctly
    auto cosTheta = glm::dot(-directionFromIntersectionToLight, lightDirection);
    auto theta = glm::acos(cosTheta);

    // determine intensity, based on location on spot cone
    glm::vec4 intensity;
    float inner = light.angle - light.penumbra;
    if (theta <= inner)
    {
        intensity = light.color;
    }
    else if
    (
        theta > inner
        && theta <= light.angle
    )
    {
        // inside the penumbra, need to apply falloff
        float falloff = -2 * std::pow(theta - inner, 3) / std::pow(light.penumbra, 3) +
                        3 * std::pow(theta - inner, 2) / std::pow(light.penumbra, 2);
        intensity = light.color * (1 - falloff);
    }
    else // theta > light.angle
    {
        return glm::vec4(0.f);
    }

    // if the light is within the cone, see if it's a shadow
    auto distanceToLight = glm::distance(light.pos, intersectionWorld);
    bool isShadow = RayTracer::isShadowed(
            light.pos,
            distanceToLight,
            glm::vec4(directionFromIntersectionToLight),
            glm::vec4(intersectionWorld),
            scene);
    if (isShadow)
    {
        // if this is a shadow, then no light contribution
        return glm::vec4(0.f);
    }

    // calculate attenuation
    float c1 = light.function.x;
    float c2 = light.function.y;
    float c3 = light.function.z;
    float attenuation = std::min(1.f, 1.f / (c1 + distance * c2 + (distance * distance) * c3));

    return phong(
            intensity,
            attenuation,
            directionFromIntersectionToLight,
            directionToCamera,
            intersectionWorld,
            normalWorld,
            shape,
            scene);
}

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

    // check if an object blocks ours
    bool isShadow = RayTracer::isShadowed(
            light.pos,
            distanceToLight,
            directionFromIntersectionToLight,
            glm::vec4(intersectionWorld),
            scene);
    if (isShadow)
    {
        // if this is a shadow, then no light contribution
        return glm::vec4(0.f);
    }

    float attenuation = 1.f; // directional lights don't attenuate
    return phong(
            light.color,
            attenuation,
            directionFromIntersectionToLight,
            directionToCamera,
            intersectionWorld,
            normalWorld,
            shape,
            scene);
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
                illumination +=
                        illuminationFromPointLight(light, intersectionWorld, normalWorld, directionToCamera, shape, scene);
                continue;
            case LightType::LIGHT_DIRECTIONAL:
                illumination +=
                        illuminationFromDirectionalLight(light, intersectionWorld, normalWorld, directionToCamera, shape, scene);
                continue;
            case LightType::LIGHT_SPOT:
                illumination +=
                        illuminationFromSpotLight(light, intersectionWorld, normalWorld, directionToCamera, shape, scene);
                continue;
            case LightType::LIGHT_AREA:
                illumination +=
                        illuminationFromAreaLight(light, intersectionWorld, normalWorld, directionToCamera, shape, scene);
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

// helper function to handle the diffuse and specular terms
// also handles the texture within that diffuse term
glm::vec4 RayTracer::phong(
        glm::vec4 lightColor,
        float attenuation,
        glm::vec4 directionFromIntersectionToLight,
        glm::vec4 directionToCamera,
        glm::vec4 intersectionWorld,
        glm::vec4 normalWorld,
        const RenderShapeData &shape,
        const RayTraceScene &scene)
{
    float kd = scene.getGlobalData().kd;
    float ks = scene.getGlobalData().ks;
    auto material = shape.primitive.material;

    glm::vec4 illumination(0.f);

    // calculate diffuse term
    auto dotDiffuse = glm::dot(normalWorld, directionFromIntersectionToLight);
    if (dotDiffuse > 0) // ensure not facing away
    {
        auto diffuse = (kd * material.cDiffuse);
        // commenting out texture stuff bc 4d textures??????
//        if (material.textureMap.isUsed)
//        {
//            glm::vec4 pObject = shape.inverseCTM * glm::vec4(intersectionWorld, 1.f);
//            diffuse = interpolateTexture(pObject, shape, diffuse);
//        }
        illumination += (attenuation * lightColor) * dotDiffuse * diffuse;
    }

    // add specular term
    auto reflectedDirOverNormal =
            2 * glm::dot(directionFromIntersectionToLight, normalWorld) * normalWorld -
            directionFromIntersectionToLight;
    auto dotSpecular = glm::dot(reflectedDirOverNormal, directionToCamera);
    auto toPow = std::pow(dotSpecular, material.shininess);
    if (dotSpecular > 0) {
        illumination += (attenuation * lightColor) * toPow * (ks * material.cSpecular);
    }

    return illumination;
}

// EXTRA CREDIT -> AREA LIGHT
glm::vec4  RayTracer::illuminationFromAreaLight(
        const SceneLightData &light,
        glm::vec4 intersectionWorld,
        glm::vec4 normalWorld,
        glm::vec4  directionToCamera,
        const RenderShapeData &shape,
        const RayTraceScene &scene
) {
    // select a random point within the light's height and width
    float width = light.width;
    float height = light.height;
    float x = ((float) rand() / (float) RAND_MAX) * width - width / 2.f;
    float y = ((float) rand() / (float) RAND_MAX) * height - height / 2.f;
    glm::vec4 lightPosition = light.pos + glm::vec4(x, y, 0.f, 0.f);

    auto directionFromIntersectionToLight = lightPosition - intersectionWorld;
    directionFromIntersectionToLight = glm::normalize(directionFromIntersectionToLight);

    // check if this light is blocked by an object
    auto distanceToLight = glm::distance(lightPosition, intersectionWorld);
    bool isShadow = RayTracer::isShadowed(
            lightPosition,
            distanceToLight,
            glm::vec4(directionFromIntersectionToLight),
            glm::vec4(intersectionWorld),
            scene);
    if (isShadow)
    {
        // if this is a shadow, then show a ray to a random point in the light
        return glm::vec4(0.f);
    }

    // calculate attenuation
    float c1 = light.function.x;
    float c2 = light.function.y;
    float c3 = light.function.z;
    float attenuation = std::min(1.f, 1.f / (c1 + distanceToLight * c2 + (distanceToLight * distanceToLight) * c3));

    return phong(
            light.color,
            attenuation,
            directionFromIntersectionToLight,
            directionToCamera,
            intersectionWorld,
            normalWorld,
            shape,
            scene);
}
