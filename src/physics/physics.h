//
// Created by Michael Foiani on 12/13/23.
//

#ifndef PROJECTS_RAY_PHYSICS_H
#define PROJECTS_RAY_PHYSICS_H


#include "utils/sceneparser.h"

class Physics {
public:
    static bool checkForSphereCollision(RenderShapeData &currentShape, RenderShapeData &shape);

    static bool checkForConeCollision(RenderShapeData &currentShape, RenderShapeData &shape);

    static bool checkForCylinderCollision(RenderShapeData &currentShape, RenderShapeData &shape);

    static bool checkForCubeCollision(RenderShapeData &currentShape, RenderShapeData &shape);

    static void updateShapePositions(std::vector<RenderShapeData> &shapes);

    static void handleCollisions(std::vector<RenderShapeData> &shapes);
};

#endif //PROJECTS_RAY_PHYSICS_H
