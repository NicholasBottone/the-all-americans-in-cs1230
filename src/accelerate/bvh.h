#include "raytracer/raytracer.h"

#ifndef PROJECTS_RAY_BVH_H

class bvh
{
public:
    bvh(const std::vector<KdShape> &shapes, int dimension);

    std::vector<KdShape> shapes;
    int dimension;
    BoundingRegion region{};
    bvh *leftChild;
    bvh *rightChild;
};


#define PROJECTS_RAY_BVH_H

#endif //PROJECTS_RAY_BVH_H
