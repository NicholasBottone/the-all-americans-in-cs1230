
#include <QList>
#include <QtConcurrent>
#include "../raytracer/raytracer.h"

struct pixelRoutineArgs {
    glm::vec4 pCamera;
    glm::vec4 dCamera;
    const RayTraceScene &scene;
    RayTracer *rt;
};
static RGBA pixelRoutine(pixelRoutineArgs args);

void RayTracer::renderParallel(RGBA *imageData, const RayTraceScene &scene)
{
    Camera camera = scene.getCamera();
    float cameraDepth = 1.f;
    float viewplaneHeight = 2.f*cameraDepth*std::tan(camera.getHeightAngle() / 2.f);
    float viewplaneWidth = cameraDepth*viewplaneHeight*((float)scene.width()/(float)scene.height());

    QList<pixelRoutineArgs> l{};
    for (int imageRow = 0; imageRow < scene.height(); imageRow++) {
        for (int imageCol = 0; imageCol < scene.width(); imageCol++) {
            float xCameraSpace = viewplaneWidth *
                                 (-.5f + (imageCol + .5f) / scene.width());
            float yCameraSpace = viewplaneHeight *
                                 (-.5f + (imageRow + .5f) / scene.height());

            glm::vec4 pixelDirCamera{xCameraSpace, -yCameraSpace, -cameraDepth, 0.f}; //w=0 for dir
            glm::vec4 eyeCamera{0.f, 0.f, 0.f, 1.f}; // w=1.f for point
            pixelRoutineArgs args{
                    eyeCamera,
                    pixelDirCamera,
                    scene,
                    this
            };
            l.append(args);

        }
    }
    QList<RGBA> pixels = QtConcurrent::blockingMapped(l, pixelRoutine);
    QtConcurrent::blockingMap(l, pixelRoutine);
    int index = 0;
    for (RGBA p : pixels) {
        imageData[index++] = p;
    }

    if (m_enableAntiAliasing)
    {
        filterBlur(imageData, scene.width(), scene.height());
    }
}


RGBA pixelRoutine(pixelRoutineArgs args)
{
    auto eyeCamera = args.pCamera;
    auto pixelDirCamera = args.dCamera;
    auto scene = args.scene;
    auto rt = args.rt;

    // convert camera space to world space
    auto inv = scene.getCamera().getInverseViewMatrix();
    glm::vec4 pWorld = inv * eyeCamera;
    glm::vec4 dWorld = glm::normalize(inv * pixelDirCamera);

    if (rt->m_enableDepthOfField)
    {
        // if we're doing depth of field, we need to shoot multiple rays, see camera.cpp
        return RayTracer::toRGBA(rt->secondaryRays(pWorld, dWorld, scene));
    }
    if (rt->m_enableSuperSample)
    {
        // if we're doing super sampling, we need to shoot multiple rays, see raytracer.cpp
        return rt->superSample(eyeCamera, pixelDirCamera, scene);
    }

    // shoot ray!
    RGBA pixel = RayTracer::toRGBA(rt->getPixelFromRay(pWorld, dWorld, scene, 0));
    return pixel;
}
