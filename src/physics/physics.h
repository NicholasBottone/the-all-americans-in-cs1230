//
// Created by Michael Foiani on 12/13/23.
//

#ifndef PROJECTS_RAY_PHYSICS_H
#define PROJECTS_RAY_PHYSICS_H


#include "utils/sceneparser.h"

class Physics {
public:
    static void updateShapePositions(std::vector<RenderShapeData> &shapes);

    static void handleCollisions(std::vector<RenderShapeData> &shapes);

    static bool checkForSphereCollision(RenderShapeData *currentShape, RenderShapeData *otherShape);

    static bool checkForConeCollision(RenderShapeData *currentShape, RenderShapeData *otherShape);

    static bool checkForCylinderCollision(RenderShapeData *currentShape, RenderShapeData *otherShape);

    static bool checkForCubeCollision(RenderShapeData *currentShape, RenderShapeData *otherShape);
};

#endif //PROJECTS_RAY_PHYSICS_H
