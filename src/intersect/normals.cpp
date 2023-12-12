//
// Created by Michael Foiani on 11/4/23.
//

#include "raytracer/raytracer.h"

glm::vec4 getConeNormal(
    glm::vec4 intersectPointObject)
{
    if (RayTracer::floatEquals(intersectPointObject.y, -.5f)) // normal for y base
    {
        return {0.f, -1.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject[3], -.5f)) // normal for w base
    {
        return {0.f, 0.f, 0.f, -1.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.y, .5f)) // normal for y top
    {
        return {0.f, 1.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject[3], .5f)) // normal for w top
    {
        return {0.f, 0.f, 0.f, 1.f};
    }

    // gradient in object space for cone top is 2x, r^2 - .5*y, 2z, r^2 - .5*w
    return glm::vec4{
        2.f * intersectPointObject.x,
        .25f - .5f * intersectPointObject.y,
        2.f * intersectPointObject.z,
        .25f - .5f * intersectPointObject[3]
    };
}

glm::vec4  getCylinderNormal(
    glm::vec4 intersectPointObject)
{
    if (RayTracer::floatEquals(intersectPointObject.y + intersectPointObject[3], -.5f)) // normal for base
    {
        return {0.f, -1.f, 0.f, -1.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.y + intersectPointObject[3], .5f)) // normal for top
    {
        return {0.f, 1.f, 0.f, 1.f};
    }

    // gradient in object space for cylinder top is 2x, 0, 2z
    return glm::vec4{
        2.f * intersectPointObject.x,
        0.f,
        2.f * intersectPointObject.z,
        0.f
    };
}

glm::vec4 getCubeNormal(
    glm::vec4 intersectPointObject)
{
    if (RayTracer::floatEquals(intersectPointObject.y, -.5f)) // neg y
    {
        return {0.f, -1.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.y, .5f)) // pos y
    {
        return {0.f, 1.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.x, -.5f)) // neg x
    {
        return {-1.f, 0.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.x, .5f)) // pos x
    {
        return {1.f, 0.f, 0.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.z, -.5f)) // neg z
    {
        return {0.f, 0.f, -1.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject.z, .5f)) // pos z
    {
        return {0.f, 0.f, 1.f, 0.f};
    }
    if (RayTracer::floatEquals(intersectPointObject[3], -.5f)) // neg w
    {
        return {0.f, 0.f, 0.f, -1.f};
    }
    if (RayTracer::floatEquals(intersectPointObject[3], .5f)) // pos w
    {
        return {0.f, 0.f, 0.f, 1.f};
    }
    return glm::vec4(0.f);
}

glm::vec4 getSphereNormal(
    glm::vec4 intersectPointObject)
{
    return {2.f * intersectPointObject.x,
            2.f * intersectPointObject.y,
            2.f * intersectPointObject.z,
            2.f * intersectPointObject.w};
}

glm::vec4 RayTracer::getNormal(
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
    return glm::vec4(0.f);
}
