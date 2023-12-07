#ifndef KDTREE_H
#define KDTREE_H
#include "utils/sceneparser.h"
#include <queue>
#include <vector>

typedef struct {
    glm::vec4 xmax;
    glm::vec4 xmin;
    glm::vec4 ymax;
    glm::vec4 ymin;
    glm::vec4 zmax;
    glm::vec4 zmin;
    glm::vec4 center;
} BoundingRegion;

typedef struct KdShape
{
    RenderShapeData shape;
    BoundingRegion region;
} KdShape;

class KdTree
{
public:

    KdTree(int pDimension, float pSplitCoord);

    bool empty;
    int dimension;
    float splitCoord;
    std::vector<KdShape> shapesWithinBounds;
    void insert(KdShape shape);

    KdTree *leftChild;
    KdTree *rightChild;

    // todo: make basis a matrix
    static BoundingRegion transformBoundingRegion(BoundingRegion region, glm::mat4 transformationMatrix, glm::vec3 basis=glm::vec3(1.0f, 1.0f, 1.0f));
};

const static BoundingRegion OBJECT_BOUNDS{
        glm::vec4(.5f, 0.f, 0.f, 1.f),
        glm::vec4(-.5f, 0.f, 0.f, 1.f),
        glm::vec4(0.f, .5f, 0.f, 1.f),
        glm::vec4(0.f, -.5f, 0.f, 1.f),
        glm::vec4(0.f, 0.f, .5f, 1.f),
        glm::vec4(0.f, 0.f, -.5f, 1.f),
        glm::vec4(0.f, 0.f, 0.f, 1.f)
};


#endif // KDTREE_H