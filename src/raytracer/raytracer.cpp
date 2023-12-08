#include <QList>
#include <QtConcurrent>
#include <iostream>
#include "raytracer.h"
#include "raytracescene.h"
#include "settings.h"
#include "mainwindow.h"
#include <QKeyEvent>
#include <QTimerEvent>


// RayTracer::RayTracer(const Config &config) : m_config(config) {}
RayTracer::RayTracer(QWidget *parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);

    // map the 1 key
    m_keyMap[Qt::Key_1]       = false;
    m_keyMap[Qt::Key_2]       = false;
    m_keyMap[Qt::Key_3]       = false;
    m_keyMap[Qt::Key_4]       = false;
    m_keyMap[Qt::Key_5]       = false;
    m_keyMap[Qt::Key_6]       = false;

}

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    if(m_enableParallelism)
    {
        renderParallel(imageData, scene);
        return;
    }

    // naive rendering
    Camera camera = scene.getCamera();
    float cameraDepth = 1.f;

    float viewplaneHeight = 2.f*cameraDepth*std::tan(camera.getHeightAngle() / 2.f);
    float viewplaneWidth = cameraDepth*viewplaneHeight*((float)scene.width()/(float)scene.height());

    for (int imageRow = 0; imageRow < scene.height(); imageRow++) {
        for (int imageCol = 0; imageCol < scene.width(); imageCol++) {
            float xCameraSpace = viewplaneWidth *
                                 (-.5f + (imageCol + .5f) / scene.width());
            float yCameraSpace = viewplaneHeight *
                                 (-.5f + (imageRow + .5f) / scene.height());

            glm::vec4 pixelDirCamera{xCameraSpace, -yCameraSpace, -cameraDepth, 0.f}; //w=0 for dir
            glm::vec4 eyeCamera{0.f, 0.f, 0.f, 1.f}; // w=1.f for point

            // convert to world space
            glm::vec4 pWorld = camera.getInverseViewMatrix() * eyeCamera;
            glm::vec4 dWorld = glm::normalize(camera.getInverseViewMatrix() * pixelDirCamera);

            // cast ray!
            glm::vec4 pixel = getPixelFromRay(pWorld, dWorld, scene);
            imageData[imageRow * scene.width() + imageCol] = toRGBA(pixel);
        }
    }
}


glm::vec4 RayTracer::getPixelFromRay(
    glm::vec4 pWorld,
    glm::vec4 dWorld,
    const RayTraceScene &scene,
    int depth)
{
    if (depth > m_maxRecursiveDepth)
    {
        return glm::vec4(0.f);
    }

    // variables from computing the intersection
    glm::vec4 closestIntersectionObj;
    glm::vec4 closestIntersectionWorld;
    RenderShapeData intersectedShape;

    if (m_enableAcceleration)
    {
        float tWorld = traverseBVH(pWorld, dWorld, intersectedShape, scene.m_bvh);
        if (tWorld == FINF)
        {
            return glm::vec4(0.f);
        }
        closestIntersectionWorld = pWorld + tWorld * dWorld;
        closestIntersectionObj = intersectedShape.inverseCTM * closestIntersectionWorld;
    }
    else
    {
        float minDist = FINF;
        // shoot a ray at each shape
        for (const RenderShapeData &shape : scene.getShapes()) {
            glm::vec4 pObject = shape.inverseCTM * pWorld;
            glm::vec4 dObject = glm::normalize(shape.inverseCTM * dWorld);

            glm::vec4 newIntersectionObj = findIntersection(pObject, dObject, shape);
            if (newIntersectionObj.w == 0) // no hit
            {
                continue;
            }

            auto newIntersectionWorld = shape.ctm * newIntersectionObj;
            float newDist = glm::distance(newIntersectionWorld, pWorld);
            if (
                    newDist < minDist // closer intersection
                    && !floatEquals(newDist, 0) // and not a self intersection
                    )
            {
                minDist = newDist;

                intersectedShape = shape;
                closestIntersectionObj = newIntersectionObj;
                closestIntersectionWorld = newIntersectionWorld;
            }
        }

        if (minDist == FINF) // no hit
        {
            return glm::vec4(0.f);
        }
    }

    glm::vec3 normalObject = getNormal(closestIntersectionObj, intersectedShape, scene);
    glm::vec3 normalWorld =
            (
                    glm::inverse(glm::transpose(intersectedShape.ctm))
                    * glm::vec4(normalObject, 0.f)
            ).xyz();

    return illuminatePixel(closestIntersectionWorld, normalWorld, -dWorld, intersectedShape, scene, depth);
}

// EXTRA CREDIT -> depth of field
glm::vec4 RayTracer::secondaryRays(glm::vec4 pWorld, glm::vec4 dWorld, RayTraceScene &scene)
{
    auto inv = scene.getCamera().getInverseViewMatrix();
    float focalLength = scene.getCamera().getFocalLength();
    float aperture = scene.getCamera().getAperture();

    glm::vec4 illumination(0.f);
    glm::vec4 focalPoint = pWorld + focalLength * dWorld;

    int TIMES = 500;
    for (int i = 0; i < TIMES; i++) {
        // generate a random number from -aperature to aperature
        float rand1 = ((float) rand() / (float) RAND_MAX) * aperture;
        rand1 *= (rand() % 2 == 0) ? 1 : -1;
        // generate another number also inside the aperature lens
        float rand2 = ((float) rand() / (float) RAND_MAX) * std::sqrt(aperture - rand1*rand1);
        rand2 *= (rand() % 2 == 0) ? 1 : -1;
        glm::vec4 randEye = (rand() % 2 == 0) ? glm::vec4(rand1, rand2, 0.f, 1.f) : glm::vec4(rand2, rand1, 0.f, 1.f);
        // convert this random point to world space
        glm::vec4 eyeWorld = inv * randEye;

        // make the ray
        glm::vec4 randomDir = glm::vec4(glm::normalize(focalPoint.xyz() - eyeWorld.xyz()), 0.f);

        illumination += getPixelFromRay(eyeWorld, randomDir, scene, 0);
    }

    return illumination / (float) TIMES;
}

void RayTracer::sceneChanged(QLabel* imageLabel) {
    // RenderData metaData;
    
    bool success = SceneParser::parse(settings.sceneFilePath, m_metaData);

    if (!success) {
        std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
        return;
    }

    int width = 576;
    int height = 432;

    // render the scene
    QImage image = QImage(width, height, QImage::Format_RGBX8888);
    image.fill(Qt::black);
    RGBA *data = reinterpret_cast<RGBA *>(image.bits());


    RayTraceScene rtScene{ width, height, m_metaData };
    this->render(data, rtScene);

    QImage flippedImage = image.mirrored(false, false);
    // make the image larger
    flippedImage = flippedImage.scaled(2*width, 2*height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(flippedImage));

    m_imageLabel = imageLabel;
}

void RayTracer::settingsChanged(QLabel* imageLabel) {
    int width = 576;
    int height = 432;

    QImage image = QImage(width, height, QImage::Format_RGBX8888);
    image.fill(Qt::black);
    RGBA *data = reinterpret_cast<RGBA *>(image.bits());

    RayTraceScene rtScene{ width, height, m_metaData };
    this->render(data, rtScene);

    QImage flippedImage = image.mirrored(false, false);
    flippedImage = flippedImage.scaled(2*width, 2*height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(flippedImage));
}

void RayTracer::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
    std::cout << "key pressed" << std::endl;
    if (m_keyMap[Qt::Key_1]) {
        std::cout << "key 1" << std::endl;
        if (settings.negative) {
            settings.xy -= settings.rotation;
        } else  {
            settings.xy += settings.rotation;
        }
        emit xyRotationChanged(settings.xy);
    }

    if (m_keyMap[Qt::Key_2]) {
        if (settings.negative) {
            settings.xz -= settings.rotation;
        } else  {
            settings.xz += settings.rotation;
        }
        emit xzRotationChanged(settings.xz);
    }

    if (m_keyMap[Qt::Key_3]) {
        if (settings.negative) {
            settings.xw -= settings.rotation;
        } else  {
            settings.xw += settings.rotation;
        }
        emit xwRotationChanged(settings.xw);
    }

    if (m_keyMap[Qt::Key_4]) {
        if (settings.negative) {
            settings.yz -= settings.rotation;
        } else  {
            settings.yz += settings.rotation;
        }
        emit yzRotationChanged(settings.yz);
    }

    if (m_keyMap[Qt::Key_5]) {
        if (settings.negative) {
            settings.yw -= settings.rotation;
        } else  {
            settings.yw += settings.rotation;
        }
        emit ywRotationChanged(settings.yw);
    }

    if (m_keyMap[Qt::Key_6]) {
        if (settings.negative) {
            settings.zw -= settings.rotation;
        } else  {
            settings.zw += settings.rotation;
        }
        emit zwRotationChanged(settings.zw);
    }
}

void RayTracer::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

