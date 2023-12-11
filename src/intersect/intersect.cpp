#include "raytracer/raytracer.h"

/**
 * @brief This source file handles intersection calculations to be used by the ray tracer.
 * The implementation for findIntersection in the RayTracer namespace is at the end of the file.
 */

// TODO: implement mesh

//updated to handle intersection in 4d
glm::vec4 intersectCircle(
        glm::vec4 p,
        glm::vec4 d,
        const RenderShapeData& shape)
{
    // implicit: x^2 + y^2 + z^2 - r^2 = 0, all directions
    float radius = 0.5f;
    float a = d.x*d.x + d.y*d.y + d.z*d.z + d[3] * d[3];
    float b = 2.f * (p.x*d.x + p.y*d.y + p.z*d.z + p[3]*d[3]);
    float c = p.x*p.x + p.y*p.y + p.z*p.z + p[3] * p[3] - radius*radius;

    float discriminant = b*b - 4*a*c;
    if (discriminant < 0) // no solution
    {
        return glm::vec4(0.f);
    }

    float t1 = (-b - std::sqrt(discriminant)) / (2.f*a);
    float t2 = (-b + std::sqrt(discriminant)) / (2.f*a);
    if (t1 <= 0 && t2 <= 0) // both behind camera
    {
        return glm::vec4(0.f);
    } else if (t1 <= 0) // t2 in front of camera
    {
        return p + t2*d;
    } else if (t2 <= 0) // t1 in front of camera
    {
        return p + t1*d;
    } else {
        float t = std::min(t1, t2);
        return p + t*d; // want best intersection point
    }
}

glm::vec4 intersectCone(
        glm::vec4 p,
        glm::vec4 d,
        const RenderShapeData& shape)
{
    float t = FINF;
    // updated to 4d
    // x^2 + y^2 - z^2 - w^2= 0, conic top
    float radius = 0.5f;
    float a = d.x*d.x + d.z*d.z - .25f*(d.y*d.y) - .25f*(d[3]*d[3]);
    float b = 2.f*(p.x*d.x + p.z*d.z) - .5f*(p.y*d.y) + .25f*d.y - .5f*(p[3]*d[3]) + .25f*d[3];
    float c = p.x*p.x + p.z*p.z - .25f*(p.y*p.y) + .25f*p.y - .25f*(p[3]*p[3]) + .25f*p[3] - 1/8.f;

    float discriminant = b*b - 4*a*c;
    if (discriminant >= 0)
    {
        float t1 = (-b - std::sqrt(discriminant)) / (2.f*a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.f*a);

        auto p1Top = p + t1 * d;
        if (
                t1 > 0 &&
                p1Top.y >= -.5f && p1Top.y <= .5f &&
                p1Top[3] >= -.5f && p1Top[3] <= .5f)

        {
            t = std::min(t1, t);
        }

        auto p2Top = p + t2 * d;
        if (
                t2 > 0 &&
                p2Top.y >= -.5f && p2Top.y <= .5f &&
                p2Top[3] >= -.5f && p2Top[3] <= .5f)

        {
            t = std::min(t2, t);
        }
    }

    // x^2 + y^2 - z^2 = 0, base w.r.t. w axis
    float twBase = (- .5f - p[3]) / d[3];
    auto pwBase = p + twBase * d;
    if (
            twBase > 0 &&
            pwBase.x*pwBase.x + pwBase.z*pwBase.z <= pwBase.y*pwBase.y
            )
    {
        t = std::min(t, twBase);
    }

    // x^2 + y^2 - z^2 = 0, base w.r.t. y axis
    float tyBase = (- .5f - p.y) / d.y;
    auto pyBase = p + tyBase * d;
    if (
        tyBase > 0 &&
        pyBase.x*pyBase.x + pyBase.z*pyBase.z <= pyBase[3]*pyBase[3]
        )
    {
        t = std::min(t, tyBase);
    }

    return t == FINF ? glm::vec4(0.f) : p + t*d;
}

glm::vec4 intersectCylinder(
        glm::vec4 p,
        glm::vec4 d,
        const RenderShapeData& shape)
{
    float t = FINF;

    // implicit: x^2 + z^2 = 0, y between -.5, 5 rectuangular side
    float radius = 0.5f;
    float a = d.x*d.x + d.z*d.z;
    float b = 2.f * (p.x*d.x + p.z*d.z);
    float c = p.x*p.x + p.z*p.z - radius*radius;

    float discriminant = b*b - 4*a*c;
    if (discriminant >= 0)
    {
        float t1 = (-b - std::sqrt(discriminant)) / (2.f*a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.f*a);

        auto p1Top = p + t1 * d;
        if (
        t1 > 0 &&
        p1Top.y >= -.5f && p1Top.y <= .5f)
        {
            t = std::min(t1, t);
        }

        auto p2Top = p + t2 * d;
        if (
            t2 > 0 &&
            p2Top.y >= -.5f && p2Top.y <= .5f)
        {
            t = std::min(t2, t);
        }
    }


    // implicit p_y + t*d_y = -.5f, top base
    float tTop = (.5f - p.y) / d.y;
    auto pTop = p + tTop * d;
    if (
        tTop > 0 &&
        pTop.x*pTop.x + pTop.z*pTop.z <= radius*radius
    )
    {
        t = std::min(t, tTop);
    }


    // implicit p_y + t*d_y = -.5f, top base
    float tBase = (- .5f - p.y) / d.y;
    auto pBase = p + tBase * d;
    if (
        tBase > 0 &&
        pBase.x*pBase.x + pBase.z*pBase.z <= radius*radius
    )
    {
        t = std::min(t, tBase);
    }

    return t == FINF ? glm::vec4(0.f) : p + t*d;
}

glm::vec4 intersectCube (
        glm::vec4 p,
        glm::vec4 d,
        const RenderShapeData& shape)
{
    // float t = FINF;
    float apothem = .5f;

    // start with x-dir
    float tmin = (-apothem - p.x) / d.x;
    float tmax = (apothem - p.x) / d.x;

    // see if it hits top or bottom
    if (tmin > tmax)
    {
        std::swap(tmin, tmax);
    }

    // y-dir
    float tymin = (-apothem - p.y) / d.y;
    float tymax = (apothem - p.y) / d.y;

    if (tymin > tymax)
    {
        std::swap(tymin, tymax);
    }

    if ((tmin > tymax) || (tymin > tmax))
    {   // no hit
        return glm::vec4(0.f);
    }

    if (tymin > tmin)
    {
        tmin = tymin;
    }
    if (tymax < tmax)
    {
        tmax = tymax;
    }

    // z-dir
    float tzmin = (-apothem - p.z) / d.z;
    float tzmax = (apothem - p.z) / d.z;

    if (tzmin > tzmax)
    {
        std::swap(tzmin, tzmax);
    }

    if ((tmin > tzmax) || (tzmin > tmax))
    {   // no hit
        return glm::vec4(0.f);
    }

    if (tzmin > tmin)
    {
        tmin = tzmin;
    }
    if (tzmax < tmax)
    {
        tmax = tzmax;
    }

    if (tmin <= 0 && tmax <= 0) // both behind camera
    {
        return glm::vec4(0.f);
    } else if (tmin > 0) // tmin in front of camera
    {
        return p + tmin*d;
    } else if (tmin <= 0) // tmax in front of camera
    {
        return p + tmax*d;
    }

    return glm::vec4(0.f);
}

/**
 * @brief Finds the intersection point of a ray and a shape.
 * The ray and shape should be in the same space for this function to work properly.
 * This function does not check if the intersection point is in front of the camera.
 * @param p, the point of the ray
 * @param d, the direction of the space
 * @param shape, the shape to be intersected with the ray
 * @return the intersection point as a vec4. If there exists no intersection, returns vec4(0.f).
 */
glm::vec4 RayTracer::findIntersection(
        glm::vec4 p,
        glm::vec4 d,
        const RenderShapeData& shape)
{
    switch(shape.primitive.type) {
        case PrimitiveType::PRIMITIVE_SPHERE:
            return intersectCircle(p, d, shape);
        case PrimitiveType::PRIMITIVE_CONE:
            return intersectCone(p, d, shape);
        case PrimitiveType::PRIMITIVE_CYLINDER:
            return intersectCylinder(p, d, shape);
        case PrimitiveType::PRIMITIVE_CUBE:
            return intersectCube(p, d, shape);
        case PrimitiveType::PRIMITIVE_MESH:
            break;
    }
    return glm::vec4(0.f);
}
