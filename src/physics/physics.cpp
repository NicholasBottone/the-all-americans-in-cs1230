//
// Created by Michael Foiani on 12/13/23.
//
#include <iostream>
#include "physics.h"

bool Physics::checkForSphereCollision(RenderShapeData &currentShape, RenderShapeData &shape)
{
    glm::vec4 currentCenter = currentShape.translation4d;
    glm::vec4 shapeCenter = shape.translation4d;
    // define a radius vector
    float radius = .5;
    float distance = glm::distance(currentCenter, shapeCenter);

    // std::cout << "distance: " << distance << std::endl;

    // update velocity
    if (distance <= radius + radius)
    {
        currentShape.velocity *= -1.f;
        // move a little in other direction so it doesn't flip again
        currentShape.translation4d = currentShape.velocity;
    }

    return distance <= radius + radius;
}

bool Physics::checkForConeCollision(RenderShapeData &currentShape, RenderShapeData &shape)
{
    return false;
}

bool Physics::checkForCylinderCollision(RenderShapeData &currentShape, RenderShapeData &shape)
{
    return false;
}

bool Physics::checkForCubeCollision(RenderShapeData &currentShape, RenderShapeData &shape)
{
    return false;
}

void Physics::handleCollisions(std::vector<RenderShapeData> &shapes) {
    for (auto &shape : shapes)
    {
        for (auto &otherShape : shapes)
        {
            if (shape.ctm == otherShape.ctm && shape.translation4d == otherShape.translation4d)
            {
                continue;
            }
            switch (shape.primitive.type)
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