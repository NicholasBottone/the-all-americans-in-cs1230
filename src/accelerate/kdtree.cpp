#include "kdtree.h"
#include "raytracer/raytracer.h"

// Constructor
KdTree::KdTree(int pDimension, float pSplitCoord) {
    empty = true;
    dimension = pDimension;
    splitCoord = pSplitCoord;

    shapesWithinBounds = std::vector<KdShape>();

    leftChild = nullptr;
    rightChild = nullptr;
}

void KdTree::insert(KdShape shape)
{
    // first, add shape to this node
    shapesWithinBounds.push_back(shape);

    if (empty) {
        empty = false;
        return;
    }

    int nextDimension = (dimension + 1) % 3;
    if (dimension == 0) // x split
    {
        if (
                shape.region.xmin.x > splitCoord
                )
            // bound box is strictly to the right, only add right
        {
            if (rightChild == nullptr) {
                rightChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is y
            }
        }
        else if (
                shape.region.xmin.x < splitCoord
                && shape.region.xmax.x > splitCoord
                )
            // bounding box overlaps center, need to add to both children
        {
            if (rightChild == nullptr) {
                rightChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is y
            }

            if (leftChild == nullptr) {
                leftChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is y
            }

        }
        else if (
                shape.region.xmax.x < splitCoord
                )
            // bounding box strictly to the left, only add left
        {
            if (leftChild == nullptr) {
                leftChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is y
            }
        }
    }

    else if (dimension == 1) // y split
    {
        if (shape.region.ymin.y > splitCoord) {
            if (rightChild == nullptr) {
                rightChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is z
            }
        }
        else if (
                shape.region.ymin.y < splitCoord
                && shape.region.ymax.y > splitCoord
                ) {
            if (rightChild == nullptr) {
                rightChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is z
            }

            if (leftChild == nullptr) {
                leftChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is z
            }
        }
        else if (
                shape.region.ymax.y < splitCoord
                ) {
            if (leftChild == nullptr) {
                leftChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is z
            }
        }
    }

    else if (dimension == 2) // z split
    {
        if (shape.region.zmin.z > splitCoord) {
            if (rightChild == nullptr) {
                rightChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is x
            }
        }
        else if (
                shape.region.zmin.z < splitCoord
                && shape.region.zmax.z > splitCoord
                ) {
            if (rightChild == nullptr) {
                rightChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is x
            }

            if (leftChild == nullptr) {
                leftChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is x
            }
        }
        else if (
                shape.region.zmax.z < splitCoord
                ) {
            if (leftChild == nullptr) {
                leftChild = new KdTree(
                        nextDimension,
                        shape.region.center[nextDimension]); // next dim is x
            }
        }
    }

    // now, add shape to children
    if (leftChild != nullptr) {
        leftChild->insert(shape);
    }
    if (rightChild != nullptr) {
        rightChild->insert(shape);
    }
}

BoundingRegion KdTree::transformBoundingRegion(BoundingRegion region, glm::mat4 transformationMatrix, glm::vec3 basis)
{
    std::vector<glm::vec4> transformedPoints = std::vector<glm::vec4>();
    transformedPoints.push_back(transformationMatrix * region.xmax);
    transformedPoints.push_back(transformationMatrix * region.xmin);
    transformedPoints.push_back(transformationMatrix * region.ymax);
    transformedPoints.push_back(transformationMatrix * region.ymin);
    transformedPoints.push_back(transformationMatrix * region.zmax);
    transformedPoints.push_back(transformationMatrix * region.zmin);

    BoundingRegion transformedRegion{
            glm::vec4(-FINF),
            glm::vec4(FINF),
            glm::vec4(-FINF),
            glm::vec4(FINF),
            glm::vec4(-FINF),
            glm::vec4(FINF),
            glm::vec4(0.f)
    }; // just init values, will be set to be correct

    // these are the new bound points, but they may have been rotated or reflected
    // this also ensures axis aligned bounding boxes, given the dots with the basis
    for (glm::vec4 point: transformedPoints) {
        if (point.x * basis.x > transformedRegion.xmax.x) {
            transformedRegion.xmax = point;
        }
        if (point.x * basis.x < transformedRegion.xmin.x) {
            transformedRegion.xmin = point;
        }
        if (point.y * basis.y > transformedRegion.ymax.y) {
            transformedRegion.ymax = point;
        }
        if (point.y * basis.y < transformedRegion.ymin.y) {
            transformedRegion.ymin = point;
        }
        if (point.z * basis.z > transformedRegion.zmax.z) {
            transformedRegion.zmax = point;
        }
        if (point.z * basis.z < transformedRegion.zmin.z) {
            transformedRegion.zmin = point;
        }
    }

    transformedRegion.center = transformationMatrix * region.center;
    return transformedRegion;
}

// TODO: return the float with the shape
float RayTracer::traverse(
        glm::vec4 p,
        glm::vec4 d,
        float tStart,
        float tEnd,
        RenderShapeData &testShape,
        KdTree *tree)
{
    if (tree == nullptr) {
        return FINF;
    }

    // leaf node
    if (    tree->shapesWithinBounds.size() <= 2 ||
            tree->leftChild == nullptr || tree->rightChild == nullptr)
    {
        float minT = FINF;
        for (const auto &shape: tree->shapesWithinBounds) {
            glm::vec4 pObject = shape.shape.inverseCTM * p;
            glm::vec4 dObject = glm::normalize(shape.shape.inverseCTM * d);

            glm::vec4 intersection = findIntersection(pObject, dObject, shape.shape);
            if (intersection.w == 0.f) {
                continue;
            }
            intersection = shape.shape.ctm * intersection;
            // check within bounds
            if (
                    intersection.x <= shape.region.xmax.x && intersection.x >= shape.region.xmin.x
                    &&
                    intersection.y <= shape.region.ymax.y && intersection.y >= shape.region.ymin.y
                    &&
                    intersection.z <= shape.region.zmax.z && intersection.z >= shape.region.zmin.z
                    )
            {
                float tWorld = (intersection.x - p.x) / d.x;
                if (tWorld < minT) {
                    minT = tWorld;
                    testShape = shape.shape;
                }
            }
        }
        return minT;
    }

    // solve for t, only in current 1d-dimension
    float t = (tree->splitCoord - p[tree->dimension]) / d[tree->dimension];

    // There are three cases:
    // 1) only intersects with left (front) child (t <= tEnd)
    // 2) only intersects with right (back) child (t <= tStart)
    // 3) intersects with both children (tStart <= t <= tEnd)
    //      on last case, we need to traverse both children,
    //      but gain a significant speedup by traversing the one that is closer first.

    if (t <= tStart && tree->rightChild != nullptr)   // case 1)
    {
        return traverse(p, d, tStart, tEnd, testShape, tree->rightChild);
    }
    else if (t >= tEnd && tree->leftChild != nullptr) // case 2)
    {
        return traverse(p, d, tStart, tEnd, testShape, tree->leftChild);
    }
    else    // case 3)
    {
        float t_hit = traverse(p, d, tStart, t, testShape, tree->leftChild);
        if (t_hit < t)
        {   // this is where we save time!
            return t_hit;
        }
        return traverse(p, d, t, tEnd, testShape, tree->rightChild);
    }
}