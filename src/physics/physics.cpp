//
// Created by Michael Foiani on 12/13/23.
//
#include <iostream>
#include "physics.h"

bool sphereCube(RenderShapeData *sphere, RenderShapeData *cube)
{
    // get center of cube
    glm::vec4 cubeCenter = cube->translation4d;
    // get the width, height, depth, & yawl of cube's box
    glm::vec4 cubeScales = glm::vec4(cube->ctm[0][0], cube->ctm[1][1], cube->ctm[2][2], cube->ctm[3][3]);

    // note: assumption that cube is axis aligned
    glm::vec4 maxes = cubeCenter + cubeScales / 2.f;
    glm::vec4 mins = cubeCenter - cubeScales / 2.f;

    // get the center of sphere
    glm::vec4 sphereCenter = sphere->translation4d;
    // get radius of sphere
    // note: assumption that sphere is not scaled (TODO: make radius adaptive)
    float radius = .5f;

    // hit detection algorithm
    float distSquared = radius * radius;
    if (sphereCenter.x < mins.x) distSquared -= (sphereCenter.x - mins.x)*(sphereCenter.x - mins.x);
    else if (sphereCenter.x > maxes.x) distSquared -= (sphereCenter.x - maxes.x)*(sphereCenter.x - maxes.x);
    if (sphereCenter.y < mins.y) distSquared -= (sphereCenter.y - mins.y)*(sphereCenter.y - mins.y);
    else if (sphereCenter.y > maxes.y) distSquared -= (sphereCenter.y - maxes.y)*(sphereCenter.y - maxes.y);
    if (sphereCenter.z < mins.z) distSquared -= (sphereCenter.z - mins.z)*(sphereCenter.z - mins.z);
    else if (sphereCenter.z > maxes.z) distSquared -= (sphereCenter.z - maxes.z)*(sphereCenter.z - maxes.z);
    if (sphereCenter.w < mins.w) distSquared -= (sphereCenter.w - mins.w)*(sphereCenter.w - mins.w);
    else if (sphereCenter.w > maxes.w) distSquared -= (sphereCenter.w - maxes.w)*(sphereCenter.w - maxes.w);

    if (distSquared > 0) {
        std::cout << "distanceSquared: " << distSquared << std::endl;

        // update velocity of the objects
        sphere->velocity *= -1.f;
        cube->velocity *= -1.f;

        // move the objects in new dir so they don't collide again
        sphere->translation4d += sphere->velocity * (1.1f);
        cube->translation4d += cube->velocity * (1.1f);
    }

    return distSquared > 0;
}

bool cubeCube(RenderShapeData *c1, RenderShapeData *c2) {
    // get the width, height, depth, & yawl of cubes boxes
    glm::vec4 cube1Scales = glm::vec4(c1->ctm[0][0], c1->ctm[1][1], c1->ctm[2][2], c1->ctm[3][3]);
    glm::vec4 cube2Scales = glm::vec4(c2->ctm[0][0], c2->ctm[1][1], c2->ctm[2][2], c2->ctm[3][3]);

    // get the center of cubes
    glm::vec4 cube1Center = c1->translation4d;
    glm::vec4 cube2Center = c2->translation4d;

    // note: assumption that cube is axis aligned
    glm::vec4 cube1Maxes = cube1Center + cube1Scales / 2.f;
    glm::vec4 cube1Mins = cube1Center - cube1Scales / 2.f;
    glm::vec4 cube2Maxes = cube2Center + cube2Scales / 2.f;
    glm::vec4 cube2Mins = cube2Center - cube2Scales / 2.f;

    // hit detection algorithm
    // see if x overlap
    bool xOverlap = cube1Maxes.x >= cube2Mins.x && cube1Mins.x <= cube2Maxes.x;
    // see if y overlap
    bool yOverlap = cube1Maxes.y >= cube2Mins.y && cube1Mins.y <= cube2Maxes.y;
    // see if z overlap
    bool zOverlap = cube1Maxes.z >= cube2Mins.z && cube1Mins.z <= cube2Maxes.z;
    // see if w overlap
    bool wOverlap = cube1Maxes.w >= cube2Mins.w && cube1Mins.w <= cube2Maxes.w;

    bool intersect = xOverlap && yOverlap && zOverlap && wOverlap;
    if (intersect) {
        std::cout << "intersect: " << intersect << std::endl;

        // update velocity of the objects, based on math, assuming the objects are the same mass
        c1->velocity *= -1.f;
        c2->velocity *= -1.f;

        // move the objects in new dir so they don't collide again
        c1->translation4d += c1->velocity * (1.1f);
        c2->translation4d += c2->velocity * (1.1f);
    }

    return intersect;
}

bool sphereSphere(RenderShapeData *s1, RenderShapeData *s2)
{
    glm::vec4 currentCenter = s1->translation4d;
    glm::vec4 shapeCenter = s2->translation4d;
    // define a radius vector
    float radius = .5;
    float distance = glm::distance(currentCenter, shapeCenter);

    // std::cout << "distance: " << distance << std::endl;

    // update velocity
    if (distance <= radius + radius)
    {
        std::cout << "distance: " << distance << std::endl;
        s1->velocity *= -1.f;
        s2->velocity *= -1.f;

        // print the new velocity
        std::cout << "s1 velocity: " << s1->velocity.x << ", " << s1->velocity.y << ", " << s1->velocity.z << ", " << s1->velocity.w << std::endl;
        std::cout << "s2 velocity: " << s2->velocity.x << ", " << s2->velocity.y << ", " << s2->velocity.z << ", " << s2->velocity.w << std::endl;

        // move the objects in new dir so they don't collide again
        s1->translation4d += s1->velocity * (1.1f);
        s2->translation4d += s2->velocity * (1.1f);
    }

    return distance <= radius + radius;
}

bool Physics::checkForSphereCollision(RenderShapeData *currentShape, RenderShapeData *otherShape)
{
    switch (otherShape->primitive.type)
    {
        case PrimitiveType::PRIMITIVE_CUBE:
            return sphereCube(currentShape, otherShape);
        case PrimitiveType::PRIMITIVE_SPHERE:
            return sphereSphere(currentShape, otherShape);
        default:
            break;
    }

    return false;
}

bool Physics::checkForConeCollision(RenderShapeData *currentShape, RenderShapeData *otherShape)
{
    return false;
}

bool Physics::checkForCylinderCollision(RenderShapeData *currentShape, RenderShapeData *otherShape)
{
    return false;
}

bool Physics::checkForCubeCollision(RenderShapeData *currentShape, RenderShapeData *otherShape)
{
    switch (otherShape->primitive.type)
    {
        case PrimitiveType::PRIMITIVE_CUBE:
            return cubeCube(currentShape, otherShape);
        case PrimitiveType::PRIMITIVE_SPHERE:
            return sphereCube(otherShape, currentShape);
        default:
            break;
    }
}

void Physics::handleCollisions(std::vector<RenderShapeData> &shapes) {
    for (int i = 0; i < shapes.size(); i++)
    {
        auto shape = &shapes[i];

        for (int j = i + 1; j < shapes.size(); j++)
        {
            auto otherShape = &shapes[j];
            switch (shape->primitive.type)
            {
                case PrimitiveType::PRIMITIVE_CONE:
                    checkForConeCollision(shape, otherShape);
                    break;
                case PrimitiveType::PRIMITIVE_CYLINDER:
                    checkForCylinderCollision(shape, otherShape);
                    break;
                case PrimitiveType::PRIMITIVE_CUBE:
                    checkForCubeCollision(shape, otherShape);
                    break;
                case PrimitiveType::PRIMITIVE_SPHERE:
                    checkForSphereCollision(shape, otherShape);
                    break;
                default:
                    break;
            }
        }
    }
}

void Physics::updateShapePositions(std::vector<RenderShapeData> &shapes)
{
    for (auto &shape : shapes)
    {
        shape.translation4d += shape.velocity;
    }
}