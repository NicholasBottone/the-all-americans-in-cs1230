#pragma once

#include <glm/glm.hpp>
#include "utils/rgba.h"
#include "utils/sceneparser.h"
#include "raytracescene.h"
#include "accelerate/kdtree.h"
#include "accelerate/bvh.h"
#include <QOpenGLWidget>
#include <QLabel>
#include <QElapsedTimer>
#include <unordered_map>

// A forward declaration for the RaytraceScene class

class RayTraceScene;

// A class representing a ray-tracer

const float FINF = std::numeric_limits<float>::infinity();
static float mediumIor = 1.0f;

struct Config {
    bool enableShadow        = false;
    bool enableReflection    = false;
    bool enableRefraction    = false;
    bool enableTextureMap    = false;
    bool enableTextureFilter = false;
    bool enableParallelism   = false;
    bool enableSuperSample   = false;
    bool enableAntiAliasing = false;
    bool enableAcceleration  = false;
    bool enableDepthOfField  = false;
    int maxRecursiveDepth    = 4;
    bool onlyRenderNormals   = false;
};

class RayTracer : public QWidget
{
        Q_OBJECT
public:
    // constructor for the config
    RayTracer(QWidget *parent = nullptr);
//     const Config &m_config;

    // Renders the scene synchronously.
    // The ray-tracer will render the scene and fill imageData in-place.
    // @param imageData The pointer to the imageData to be filled.
    // @param scene The scene to be rendered.
    void render(RGBA *imageData, const RayTraceScene &scene);

    // shadow
    bool isShadowed(glm::vec4 lightPosition, float distanceToLight, glm::vec4 directionFromIntersectionToLight,
                    glm::vec4 intersectionWorld, const RayTraceScene &scene);

    // texture
    glm::vec4 interpolateTexture(
        glm::vec4 pObject,
        const RenderShapeData &shape,
        glm::vec4 illuminationToInterpolate);

    glm::vec4 getNormal(
            glm::vec4 intersectPointObject,
            const RenderShapeData &shape,
            const RayTraceScene &scene);

    // ray tracing
    glm::vec4 getPixelFromRay(
            glm::vec4 pWorld,
            glm::vec4 dWorld,
            const RayTraceScene &scene,
            int depth = 0);

    // intersect
    glm::vec4 findIntersection(
            glm::vec4 p,
            glm::vec4 d,
            const RenderShapeData& shape, 
            bool& isHit);

    // utils
    static RGBA toRGBA(const glm::vec4 &illumination);
    static bool floatEquals(float a, float b, float epsilon = 0.0001f);

    // refracting, reflecting
    glm::vec4 refract(
            glm::vec3 intersectionWorld,
            glm::vec3 normalWorld,
            glm::vec3 incidentDir,
            const RenderShapeData &shape,
            const RayTraceScene &scene,
            int depth);
    glm::vec4 reflect(
            glm::vec3 intersectionWorld,
            glm::vec3 normalWorld,
            glm::vec3 incidentDir,
            const RenderShapeData &shape,
            const RayTraceScene &scene,
            int depth);
    glm::vec4 illuminatePixel(
            glm::vec4 intersectionWorld,
            glm::vec4 normalWorld,
            glm::vec4 directionToCamera,
            const RenderShapeData &shape,
            const RayTraceScene &scene,
            int depth);


    // shading, and helpers for each type of light
    glm::vec4
    phong(glm::vec4 lightColor, float attenuation, glm::vec4 directionFromIntersectionToLight,
          glm::vec4 directionToCamera,
          glm::vec4 intersectionWorld, glm::vec4 normalWorld, const RenderShapeData &shape, const RayTraceScene &scene);

    glm::vec4
    illuminationFromPointLight(const SceneLightData &light, glm::vec4 intersectionWorld, glm::vec4 normalWorld,
                               glm::vec4 directionToCamera, const RenderShapeData &shape,
                               const RayTraceScene &scene);

    glm::vec4 illuminationFromSpotLight(const SceneLightData &light, glm::vec4 intersectionWorld, glm::vec4 normalWorld,
                                        glm::vec4 directionToCamera, const RenderShapeData &shape,
                                        const RayTraceScene &scene);

    glm::vec4
    illuminationFromDirectionalLight(const SceneLightData &light, glm::vec4 intersectionWorld, glm::vec4 normalWorld,
                                     glm::vec4 directionToCamera, const RenderShapeData &shape,
                                     const RayTraceScene &scene);

    glm::vec4 illuminationFromAreaLight(const SceneLightData &light, glm::vec4 intersectionWorld, glm::vec4 normalWorld,
                                        glm::vec4 directionToCamera, const RenderShapeData &shape,
                                        const RayTraceScene &scene);


    // acceleration data structures
    void renderParallel(RGBA *imageData, const RayTraceScene &scene);
    float traverse(glm::vec4 p, glm::vec4 d, float tStart, float tEnd, RenderShapeData &testShape, KdTree *tree);
    float traverseBVH(glm::vec4 p, glm::vec4 d, RenderShapeData &testShape, bvh *root);

    // aliasing
    RGBA superSample(glm::vec4 eyeCamera, glm::vec4 pixelDirCamera, const RayTraceScene &scene);
    void filterBlur(RGBA *imageData, int width, int height, float blurRadius = 3.f);

    // depth of field
    glm::vec4 secondaryRays(glm::vec4 pWorld, glm::vec4 dWorld, RayTraceScene &scene);

    // Old m_config

    bool m_enableShadow        = true;
    bool m_enableReflection    = true;
    bool m_enableRefraction    = false;
    bool m_enableTextureMap    = false;
    bool m_enableAcceleration  = false;
    bool m_enableParallelism   = true;
    int m_maxRecursiveDepth   = 4;
    bool m_enableAntiAliasing = false;
    bool m_enableDepthOfField  = false;
    bool m_enableSuperSample   = false;

    void sceneChanged(QLabel* imageLabel);
    void settingsChanged(QLabel* imageLabel);
    
    RenderData m_metaData;
    std::unordered_map<Qt::Key, bool> m_keyMap;
    QElapsedTimer m_elapsedTimer;  
    QLabel* m_imageLabel;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    glm::mat4 getRotationMatrix4(float angle, glm::vec3 axis3, glm::vec3 axisW);

    RGBA *m_imageData;
    RGBA *m_voxelBuffer;

    int m_width;
    int m_height;
    int m_depth;

    QImage m_image;
signals:
    void xyRotationChanged(float value);
    void xzRotationChanged(float value);
    void xwRotationChanged(float value);
    void yzRotationChanged(float value);
    void ywRotationChanged(float value);
    void zwRotationChanged(float value);
    void rotationChanged(float value);
};

