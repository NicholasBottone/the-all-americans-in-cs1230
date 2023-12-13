
#include <QList>
#include <QtConcurrent>
#include <iostream>
#include "../raytracer/raytracer.h"
#include "../vec4ops/vec4ops.h"
#include "settings.h"

struct pixelRoutineArgs {
    glm::vec4 pWorld;
    glm::vec4 dWorld;
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
    float viewplaneDepth = cameraDepth*viewplaneHeight * ((float)scene.depth() / (float)scene.height());

    QList<pixelRoutineArgs> l{};
    // for (int imageRow = 0; imageRow < scene.height(); imageRow++) {
    //     for (int imageCol = 0; imageCol < scene.width(); imageCol++) {
    //         float xCameraSpace = viewplaneWidth *
    //                              (-.5f + (imageCol + .5f) / scene.width());
    //         float yCameraSpace = viewplaneHeight *
    //                              (-.5f + (imageRow + .5f) / scene.height());

    //         glm::vec4 pixelDirCamera{xCameraSpace, -yCameraSpace, -cameraDepth, 0.f}; //w=0 for dir
    //         glm::vec4 eyeCamera{0.f, 0.f, 0.f, 1.f}; // w=1.f for point
    //         pixelRoutineArgs args{
    //                 eyeCamera,
    //                 pixelDirCamera,
    //                 scene,
    //                 this
    //         };
    //         l.append(args);

    //     }
    // }
    for (int imageRow = 0; imageRow < scene.height(); imageRow++) {
        for (int imageCol = 0; imageCol < scene.width(); imageCol++) {
            // FIXME: for now, use W slider ad depth
            int imageDepth = (int) ((settings.w + 100.f) * (5.f / 2.f));
            // compute the ray
            float x = (imageCol - scene.width()/2.f) * viewplaneWidth / scene.width();
            float y = (imageRow - scene.height()/2.f) * viewplaneHeight / scene.height();
            float z = (imageDepth - scene.depth()/2.f) * viewplaneDepth / scene.depth();

            glm::vec4 pWorld = Vec4Ops::transformPoint4(glm::vec4(0.f), camera.getViewMatrix(), camera.getTranslationVector());
            glm::vec4 dWorld = Vec4Ops::transformDir4(glm::vec4(x, y, z, -1.0), camera.getViewMatrix());

             pixelRoutineArgs args{
                     pWorld,
                     dWorld,
                     scene,
                     this
             };
             l.append(args);
        }
    }
    QList<RGBA> pixels = QtConcurrent::blockingMapped(l, pixelRoutine);
    QtConcurrent::blockingMap(l, pixelRoutine);

    // get the slice relating to z == 0 and set it into int the iamge data array

    // int currentSlice = settings.w + 100.f * (5.f / 2.f);
    int index = 0;
    for (RGBA p : pixels) {
        imageData[index++] = p;
    }

    std::cout << "done rendering" << std::endl;

    if (m_enableAntiAliasing)
    {
        filterBlur(imageData, scene.width(), scene.height());
    }
}


RGBA pixelRoutine(pixelRoutineArgs args)
{
    auto pWorld = args.pWorld;
    auto dWorld = args.dWorld;
    auto scene = args.scene;
    auto rt = args.rt;

    // shoot ray!
    RGBA pixel = RayTracer::toRGBA(rt->getPixelFromRay(pWorld, dWorld, scene, 0));
    return pixel;
}
