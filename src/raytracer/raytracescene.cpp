#include <stdexcept>
#include "raytracescene.h"
#include "utils/sceneparser.h"
#include "raytracer.h"
#include <iostream>

RayTraceScene::RayTraceScene(int width, int height, const RenderData &metaData, int depth) :
    m_camera(* new Camera(metaData.cameraData))
{
    // Optional TODO: implement this. Store whatever you feel is necessary.
    m_width = width;
    m_height = height;
    m_depth = depth;
    m_sceneGlobalData = metaData.globalData;
    m_shapes = metaData.shapes;
    m_lights = metaData.lights;

    // populate the kd tree
    m_kdTree = nullptr;
    std::vector<KdShape> shapes;
    for (const auto& shape : metaData.shapes) {
        KdShape s{
                shape,
                KdTree::transformBoundingRegion(OBJECT_BOUNDS, shape.ctm)
        };
        shapes.push_back(s);
    }
    m_bvh = new bvh(shapes, 0);
}

const int& RayTraceScene::width() const {
    // Optional TODO: implement the getter or make your own design
    return m_width;
}

const int& RayTraceScene::height() const {
    // Optional TODO: implement the getter or make your own design
    return m_height;
}


const int& RayTraceScene::depth() const {
    // Optional TODO: implement the getter or make your own design
    return m_depth;
}

const SceneGlobalData& RayTraceScene::getGlobalData() const {
    // Optional TODO: implement the getter or make your own design
    return m_sceneGlobalData;
}

const std::vector<RenderShapeData> RayTraceScene::getShapes() const {
    return m_shapes;
}

const std::vector<SceneLightData> RayTraceScene::getLights() const {
    return m_lights;
}

const Camera& RayTraceScene::getCamera() const {
    // Optional TODO: implement the getter or make your own design
    return m_camera;
}
