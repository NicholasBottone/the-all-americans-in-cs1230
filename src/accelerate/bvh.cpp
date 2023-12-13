#include "raytracer/raytracer.h"
#include "bvh.h"

bvh::bvh(
        const std::vector<KdShape>& p_shapes,
        int p_dimension)
{
    dimension = p_dimension;
    if (p_shapes.empty()) {
        return;
    }

    // compute the new bouding ragion from the shapes and add shapes to the root
    BoundingRegion tmp {
            glm::vec4(-FINF, -FINF, -FINF, 1.f),
            glm::vec4(FINF, FINF, FINF, 1.f),
            glm::vec4(-FINF, -FINF, -FINF, 1.f),
            glm::vec4(FINF, FINF, FINF, 1.f),
            glm::vec4(-FINF, -FINF, -FINF, 1.f),
            glm::vec4(FINF, FINF, FINF, 1.f),
            glm::vec4(0.f, 0.f, 0.f, 1.f)
    };
    shapes = std::vector<KdShape>();
    for (const auto& shape : p_shapes) {
        tmp.xmax = glm::max(tmp.xmax, shape.region.xmax);
        tmp.xmin = glm::min(tmp.xmin, shape.region.xmin);
        tmp.ymax = glm::max(tmp.ymax, shape.region.ymax);
        tmp.ymin = glm::min(tmp.ymin, shape.region.ymin);
        tmp.zmax = glm::max(tmp.zmax, shape.region.zmax);
        tmp.zmin = glm::min(tmp.zmin, shape.region.zmin);
        tmp.center.x = (tmp.xmax.x + tmp.xmin.x) / 2.f;
        tmp.center.y = (tmp.ymax.y + tmp.ymin.y) / 2.f;
        tmp.center.z = (tmp.zmax.z + tmp.zmin.z) / 2.f;

        shapes.push_back(shape);
    }
    region = tmp;

    // split the shapes into two groups, if more than two shapes
    if (shapes.size() <= 2) {
        return;
    }
    std::vector<KdShape> leftShapes;
    std::vector<KdShape> rightShapes;
    for (const auto& shape : shapes) {
        if (shape.region.center[dimension] < region.center[dimension]) {
            leftShapes.push_back(shape);
        }
        else if (shape.region.center[dimension] > region.center[dimension]) {
            rightShapes.push_back(shape);
        } else {
            if (leftShapes.size() < rightShapes.size()) {
                leftShapes.push_back(shape);
            } else {
                rightShapes.push_back(shape);
            }
        }
    }

    // make the children
    leftChild = new bvh(leftShapes, (dimension + 1) % 3);
    rightChild = new bvh(rightShapes, (dimension + 1) % 3);
}

float intersectRegion(
        glm::vec4 p,
        glm::vec4 d,
        BoundingRegion region)
{
    float tXmin = (region.xmin.x - p.x) / d.x;
    float tXmax = (region.xmax.x - p.x) / d.x;
    float tYmin = (region.ymin.y - p.y) / d.y;
    float tYmax = (region.ymax.y - p.y) / d.y;
    float tZmin = (region.zmin.z - p.z) / d.z;
    float tZmax = (region.zmax.z - p.z) / d.z;

    float tMin = std::max(std::max(std::min(tXmin, tXmax), std::min(tYmin, tYmax)), std::min(tZmin, tZmax));
    float tMax = std::min(std::min(std::max(tXmin, tXmax), std::max(tYmin, tYmax)), std::max(tZmin, tZmax));

    if (tMin > tMax) {
        return FINF;
    }
    return tMin;
}

void updateAfterCollision(RenderShapeData& objA, RenderShapeData& objB) {
    glm::vec3 vA_prime = ((objA.mass - objB.mass) * objA.velocity + 2 * objB.mass * objB.velocity) / (objA.mass + objB.mass);
    glm::vec3 vB_prime = ((objB.mass - objA.mass) * objB.velocity + 2 * objA.mass * objA.velocity) / (objA.mass + objB.mass);

    objA.velocity = glm::vec4(vA_prime, 0.f);
    objB.velocity = glm::vec4(vB_prime, 0.f);
    
    objA.position += objA.velocity;
    objB.position += objB.velocity;

}

float RayTracer::traverseBVH(
        glm::vec4 p,
        glm::vec4 d,
        RenderShapeData &testShape,
        bvh *root)
{
    std::vector<bvh*> stack = std::vector<bvh*>();
    stack.push_back(root);
    float minT = FINF;

    while (!stack.empty())
    {
        auto current = *stack.back();
        stack.pop_back();

        if (current.leftChild == nullptr && current.rightChild == nullptr) {
            for (const auto &shape: current.shapes) {
                glm::vec4 pObject = shape.shape.inverseCTM * p;
                glm::vec4 dObject = glm::normalize(shape.shape.inverseCTM * d);
                bool isHit = false;
                glm::vec4 intersection = findIntersection(pObject, dObject, shape.shape, isHit);
                if (intersection.w == 0.f) {
                    continue;
                }
                intersection = shape.shape.ctm * intersection;
                // check within bounds
                float tWorld = (intersection.x - p.x) / d.x;
                if (tWorld < minT)
                {
                    minT = tWorld;
                    testShape = shape.shape;
                }
            }
        } else {
            float leftIntersect = intersectRegion(p, d, current.leftChild->region);
            float rightIntersect = intersectRegion(p, d, current.rightChild->region);
            if (leftIntersect != FINF && rightIntersect != FINF) {
                if (leftIntersect < rightIntersect) {
                    stack.push_back(current.rightChild);
                    stack.push_back(current.leftChild);
                } else {
                    stack.push_back(current.leftChild);
                    stack.push_back(current.rightChild);
                }
            } else if (leftIntersect != FINF) {
                stack.push_back(current.leftChild);
            } else if (rightIntersect != FINF) {
                stack.push_back(current.rightChild);
            }
        }
    }

    return minT;
}
