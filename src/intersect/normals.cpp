//
// Created by Michael Foiani on 11/4/23.
//

#include "raytracer/raytracer.h"

glm::vec3 getConeNormal(
        glm::vec4 intersectPointObject)
{
    if (RayTracer::floatEquals(intersectPointObject.y, -.5f)) // normal for base
    {
        return {0.f, -1.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.y, .5f)) // normal for top
    {
        return {0.f, 1.f, 0.f};
    }

    // gradient in object space for cone top is 2x, r^2 - .5*y, 2z
    return glm::vec3{
            2.f * intersectPointObject.x,
            .25f - .5f * intersectPointObject.y,
            2.f * intersectPointObject.z
    };
}

glm::vec3  getCylinderNormal(
        glm::vec4 intersectPointObject)
{
    if (RayTracer::floatEquals(intersectPointObject.y, -.5f)) // normal for base
    {
        return {0.f, -1.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.y, .5f)) // normal for top
    {
        return {0.f, 1.f, 0.f};
    }

    // gradient in object space for cylinder top is 2x, 0, 2z
    return glm::vec3{
            2.f * intersectPointObject.x,
            0.f,
            2.f * intersectPointObject.z
    };
}

glm::vec3 getCubeNormal(
        glm::vec4 intersectPointObject)
{
    if (RayTracer::floatEquals(intersectPointObject.y, -.5f)) // neg y
    {
        return {0.f, -1.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.y, .5f)) // pos y
    {
        return {0.f, 1.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.x, -.5f)) // neg x
    {
        return {-1.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.x, .5f)) // pos x
    {
        return {1.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.z, -.5f)) // neg z
    {
        return {0.f, 0.f, -1.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.z, .5f)) // pos z
    {
        return {0.f, 0.f, 1.f};
    }
    return glm::vec3(0.f);
}

glm::vec4 getSphereNormal(
        glm::vec4 intersectPointObject)
{
    return {2.f * intersectPointObject.x,
            2.f * intersectPointObject.y,
            2.f * intersectPointObject.z, 
            2.f * intersectPointObject.w};
}

glm::vec3 RayTracer::getNormal(
        glm::vec4 intersectPointObject,
        const RenderShapeData& shape,
        const RayTraceScene &scene)
{
    switch(shape.primitive.type)
    {
        case PrimitiveType::PRIMITIVE_SPHERE:
            // gradient in object space for sphere is 2x, 2y, 2z
            // return 2.f * intersectPointObject;
            return getSphereNormal(intersectPointObject);
        case PrimitiveType::PRIMITIVE_CONE:
            return getConeNormal(intersectPointObject);
        case PrimitiveType::PRIMITIVE_CYLINDER:
            return getCylinderNormal(intersectPointObject);
        case PrimitiveType::PRIMITIVE_CUBE:
            return getCubeNormal(intersectPointObject);
        case PrimitiveType::PRIMITIVE_MESH:
            break;
    }
    return glm::vec3(0.f);
}