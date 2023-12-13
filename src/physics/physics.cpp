//
// Created by Michael Foiani on 12/13/23.
//
#include "physics.h"

bool Physics::checkForSphereCollision(RenderShapeData &currentShape, RenderShapeData &shape)
{
    glm::vec4 currentCenter = currentShape.translation4d;
    glm::vec4 shapeCenter = shape.translation4d;
    // define a radius vector
    glm::vec4 radiusVector = {.5f, 0, 0, 0};
    glm::vec4 r1 = currentShape.ctm * radiusVector;
    glm::vec4 r2 = shape.ctm * radiusVector;
    float distance = glm::distance(currentCenter, shapeCenter);

    // update velocity
    if (distance <= r1.x + r2.x)
    {
        currentShape.velocity = -currentShape.velocity;
        shape.velocity = -shape.velocity;
    }

    return distance <= r1.x + r2.x;
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