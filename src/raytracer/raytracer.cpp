#include <QList>
#include <QtConcurrent>
#include <iostream>
#include <cstdlib>
#include "raytracer.h"
#include "raytracescene.h"
#include "settings.h"
#include "mainwindow.h"
#include <QKeyEvent>
#include <QTimerEvent>
#include "vec4ops/vec4ops.h"

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

    m_width = 576;
    m_height = 432;
    m_depth = 500;

    m_image = QImage(m_width, m_height, QImage::Format_RGBX8888);
//    m_camera = nullptr;
}

glm::vec4 getPt(glm::vec3 n1 , glm::vec3 n2 , float perc )
{
    glm::vec3 diff = n2 - n1;

    return glm::vec4(n1 + ( diff * perc ), 0.0f);
}

// updated to use 4D
void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    renderParallel(imageData, scene);

    if (settings.bulkOutputFolderPath.size() > 0) { // means we are doing bulk rendering
        // save the image to the bulk directory
        std::string filePath = settings.bulkOutputFolderPath + QDir::separator().toLatin1() + std::to_string(settings.currentTime) + ".png";
        saveViewportImage(filePath);
        if (settings.currentTime < settings.maxTime) { // still more to render
            // render the next frame
            settings.currentTime++;
            emit settingsChanged(m_imageLabel); // emit to allow the UI to update then render the next frame
        } else { // done rendering
            // assemble the video
            saveFFMPEGVideo(settings.bulkOutputFolderPath);
            settings.currentTime = 0;
            settings.bulkOutputFolderPath = "";
        }
    }
    emit cameraPositionChanged(m_metaData.cameraData.pos); 
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

    float minDist = FINF;
    // shoot a ray at each shape
    for (const RenderShapeData &shape : scene.getShapes()) {
        glm::vec4 pObject = Vec4Ops::inverseTransformPoint4(pWorld, shape.inverseCTM, shape.inverseTranslation4d);
        glm::vec4 dObject = glm::normalize(Vec4Ops::inverseTransformDir4(dWorld, shape.inverseCTM));
//        std::cout << "pObject: " << pObject.x << ", " << pObject.y << ", " << pObject.z << ", " << pObject.w << std::endl;
//        std::cout << "dObject: " << dObject.x << ", " << dObject.y << ", " << dObject.z << ", " << dObject.w << std::endl;
        bool isHit = false;
        glm::vec4 newIntersectionObj = findIntersection(pObject, dObject, shape, isHit);
        if (!isHit) // no hit
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

    glm::vec4 normalObject = glm::normalize(getNormal(closestIntersectionObj, intersectedShape, scene));
    // update
    glm::vec4 normalWorld = glm::inverse(glm::transpose(intersectedShape.ctm)) * glm::vec4(normalObject);

    return illuminatePixel(closestIntersectionWorld, normalWorld, -dWorld, intersectedShape, scene, depth);
}

void RayTracer::sceneChanged(QLabel* imageLabel) {
    // RenderData metaData;
    
    bool success = SceneParser::parse(settings.sceneFilePath, m_metaData);

    if (!success) {
        std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
        m_image.fill(Qt::black);
        imageLabel->setPixmap(QPixmap::fromImage(m_image));
        m_imageData = reinterpret_cast<RGBA *>(m_image.bits());
        return;
    }

    // render the scene
    m_imageData = reinterpret_cast<RGBA *>(m_image.bits());

    RayTraceScene rtScene{ m_width, m_height, m_metaData, m_depth };
    this->render(m_imageData, rtScene);

    QImage flippedImage = m_image.mirrored(false, false);
    // make the image larger
    flippedImage = flippedImage.scaled(m_width, m_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(flippedImage));

    m_imageLabel = imageLabel;
    // m_image = image;
}

void RayTracer::settingsChanged(QLabel* imageLabel) {
    if (settings.sceneFilePath.size() == 0) {
        // no scene loaded
        m_image.fill(Qt::black);
        imageLabel->setPixmap(QPixmap::fromImage(m_image));
        m_imageData = reinterpret_cast<RGBA *>(m_image.bits());
        return;
    }

    int width = 576;
    int height = 432;

    QImage image = QImage(width, height, QImage::Format_RGBX8888);
    image.fill(Qt::black);
    m_imageData = reinterpret_cast<RGBA *>(image.bits());

    RayTraceScene rtScene{ m_width, m_height, m_metaData, m_depth };
    Camera camera = rtScene.getCamera();
    if (m_controlPointIndex % 3 == 0) {
        m_controlPoints = camera.m_controlPoints;
    }

    auto P1 = m_controlPoints[m_controlPointIndex];
    auto P2 = m_controlPoints[m_controlPointIndex];
    auto P3 = m_controlPoints[m_controlPointIndex];
    auto P4 = m_controlPoints[m_controlPointIndex];

    glm::vec4 xa = getPt(P1, P2, settings.currentTime);
    glm::vec4 xb = getPt(P2, P3, settings.currentTime);
    glm::vec4 xc = getPt(P3, P4, settings.currentTime);

    // Calculate points on the lines between the above points
    glm::vec4 xm = getPt(xa, xb, settings.currentTime);
    glm::vec4 xn = getPt(xb, xc, settings.currentTime);

    // Calculate the final point on the Bezier curve
    glm::vec4 pointOnCurve = getPt(xm, xn, settings.currentTime);
    m_metaData.cameraData.pos = pointOnCurve;

    settings.xy += 4.f;
    if (m_controlPointIndex % 1 == 0) {
        settings.xz += 8.f;
    }
    if (m_controlPointIndex % 3 == 0){
        settings.yz += 8.f;
    }
    this->render(m_imageData, rtScene);

    QImage flippedImage = image.mirrored(false, false);
    flippedImage = flippedImage.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(flippedImage));
    m_controlPointIndex++;

    
    // QTimer::singleShot(3500, this, [this, imageLabel]() {
    //     // This code will be executed after a 2-second delay
    //     emit rotationChanged(settings.rotation);
    // });
    m_image = image;
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

void RayTracer::saveViewportImage(std::string filePath) {
    QImage image = QImage((uchar *) m_imageData, 576, 432, QImage::Format_RGBX8888);
    image.save(QString::fromStdString(filePath));
}

void RayTracer::saveFFMPEGVideo(std::string filePath) {
    std::string directory = filePath + QDir::separator().toLatin1();
    std::string command = "ffmpeg -framerate 30 -pattern_type glob -i '" + directory + "*.png' -c:v libx264 -pix_fmt yuv420p '" + directory + "video.mp4'";
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to assemble video." << std::endl;
    }
}
