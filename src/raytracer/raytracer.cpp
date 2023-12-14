#include <QList>
#include <QtConcurrent>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "raytracer.h"
#include "raytracescene.h"
#include "settings.h"
#include "mainwindow.h"
#include <QKeyEvent>
#include <QTimerEvent>
#include "vec4ops/vec4ops.h"
#include "physics/physics.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // Include this header for glm::rotate

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
        std::string paddedTime = std::to_string(settings.currentTime);
        paddedTime = std::string(4 - paddedTime.length(), '0') + paddedTime;
        std::string filePath = settings.bulkOutputFolderPath + QDir::separator().toLatin1() + paddedTime + ".png";
        saveViewportImage(filePath);
        if (settings.currentTime < settings.maxTime) { // still more to render
            // render the next frame
            if (m_enableCameraBezier) {
                Camera camera = scene.getCamera();
                if (settings.currentTime % 4 == 0) {
                    m_controlPoints = camera.m_controlPoints;
                }

                auto time = (settings.currentTime % 60)/60.f;

                auto P1 = m_controlPoints[0];
                auto P2 = m_controlPoints[1];
                auto P3 = m_controlPoints[2];
                auto P4 = m_controlPoints[3];

                glm::vec4 xa = getPt(P1, P2, time);
                glm::vec4 xb = getPt(P2, P3, time);
                glm::vec4 xc = getPt(P3, P4, time);

                // Calculate points on the lines between the above points
                glm::vec4 xm = getPt(xa, xb, time);
                glm::vec4 xn = getPt(xb, xc, time);

                // Calculate the final point on the Bezier curve
                glm::vec4 pointOnCurve = getPt(xm, xn, time);
                // std::cout << "point on curve: " << pointOnCurve.x << ", " << pointOnCurve.y << ", " << pointOnCurve.z << ", " << pointOnCurve.w << std::endl;
                // std::cout << "camera pos" << m_metaData.cameraData.pos.x << ", " << m_metaData.cameraData.pos.y << ", " << m_metaData.cameraData.pos.z << ", " << m_metaData.cameraData.pos.w << std::endl;
                
                // rotate the camera about the origin
                glm::vec4 cameraPos = m_metaData.cameraData.pos;
                if (settings.currentTime < 22) {
                glm::vec4 cameraPosRotated = glm::rotate(glm::mat4(1.f), glm::radians(10.0f), glm::vec3(0.f, 1.f, 0.f)) * glm::vec4(cameraPos.x, cameraPos.y, 0.f, 1.f);

                if (settings.currentTime % 2 == 0) {
                    cameraPosRotated = glm::rotate(glm::mat4(1.f), glm::radians(2.0f), glm::vec3(1.f, 0.f, 0.f)) * cameraPosRotated;
                }
                cameraPosRotated = glm::rotate(glm::mat4(1.f), glm::radians(-3.0f), glm::vec3(1.f, 0.f, 0.f)) * cameraPosRotated;

                // if (settings.currentTime % 3 == 0) {
                //     cameraPosRotated = glm::rotate(glm::mat4(1.f), glm::radians(4.0f), glm::vec3(0.f, 0.f, 1.f)) * cameraPosRotated;
                // }
                // cameraPosRotated = glm::rotate(glm::mat4(1.f), glm::radians(-2.0f), glm::vec3(0.f, 0.f, 1.f)) * cameraPosRotated;

                m_metaData.cameraData.pos = glm::vec4(cameraPosRotated.x, cameraPosRotated.y, cameraPos.z, 1.f);
                }
                // m_metaData.cameraData.pos = glm::vec4(pointOnCurve.x, pointOnCurve.y, pointOnCurve.z, 1.f);

                settings.xy += 4.f;
                if (settings.currentTime > 22) {
                    settings.xy -= 4.f;
                    settings.xz -= 2.f;
                }
                if (settings.currentTime % 1 == 0) {
                    settings.xz += 2.f;
                    if (settings.currentTime > 22) {
                        settings.yz -= 2.f;
                    }
                }
                if (settings.currentTime % 3 == 0){
                    settings.xz -= 3.f;
                    if (settings.currentTime > 2) {
                        settings.xz += 3.f;
                    }
                }

            }
            settings.currentTime++;
            // settings.w++;

            // update physics for moving objects
            Physics::updateShapePositions(m_metaData.shapes);
            Physics::handleCollisions(m_metaData.shapes);

        } else { // done rendering
            // assemble the video
            saveFFMPEGVideo(settings.bulkOutputFolderPath);
            settings.currentTime = 0;
            settings.bulkOutputFolderPath = "";
        }
        QTimer::singleShot(0, this, [this]() {
            settingsChanged(m_imageLabel);
        });
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
        glm::vec4 pObject = Vec4Ops::inverseTransformPoint4(pWorld, shape.inverseCTM, -shape.translation4d);
        glm::vec4 dObject = glm::normalize(Vec4Ops::inverseTransformDir4(dWorld, shape.inverseCTM));
        bool isHit = false;
        glm::vec4 newIntersectionObj = findIntersection(pObject, dObject, shape, isHit);
        if (!isHit) // no hit
        {
            continue;
        }

        auto newIntersectionWorld = Vec4Ops::transformPoint4(newIntersectionObj,shape.ctm,shape.translation4d);
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
    emit timeValueChanged(settings.currentTime);

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
    // update the camera position
    rtScene.m_camera.updateViewMatrix(m_metaData.cameraData);
    this->render(m_imageData, rtScene);

    QImage flippedImage = image.mirrored(false, false);
    flippedImage = flippedImage.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(flippedImage));
    // m_controlPointIndex++;


    // QTimer::singleShot(3500, this, [this, imageLabel]() {
    //     // This code will be executed after a 2-second delay
    //     emit rotationChanged(settings.rotation);
    // });
    m_image = image;
 }

//void RayTracer::settingsChanged(QLabel* imageLabel) {
//    emit timeValueChanged(settings.currentTime);
//
//    bool success = SceneParser::parse(settings.sceneFilePath, m_metaData); // FIXME: this is a hack to get the camera position
//
//    if (!success) {
//        std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
//        // return;
//        // render a blank image
//        QImage image = QImage(576, 432, QImage::Format_RGBX8888);
//        image.fill(Qt::black);
//        RGBA *data = reinterpret_cast<RGBA *>(image.bits());
//        m_imageData = data;
//        imageLabel->setPixmap(QPixmap::fromImage(image));
//    }
//
//    // if (settings.sceneFilePath.size() == 0) {
//    //     // no scene loaded
//    //     m_image.fill(Qt::black);
//    //     imageLabel->setPixmap(QPixmap::fromImage(m_image));
//    //     m_imageData = reinterpret_cast<RGBA *>(m_image.bits());
//    //     return;
//    // }
//
//    int width = 576;
//    int height = 432;
//
//    QImage image = QImage(width, height, QImage::Format_RGBX8888);
//    image.fill(Qt::black);
//    RGBA *data = reinterpret_cast<RGBA *>(image.bits());
//
//    RayTraceScene rtScene{ m_width, m_height, m_metaData, m_depth };
////    Camera camera = rtScene.getCamera();
////    if (settings.currentTime % 3 == 0) {
////        m_controlPoints = camera.m_controlPoints;
////    }
////
////    auto P1 = m_controlPoints[settings.currentTime];
////    auto P2 = m_controlPoints[settings.currentTime];
////    auto P3 = m_controlPoints[settings.currentTime];
////    auto P4 = m_controlPoints[settings.currentTime];
////
////    // glm::vec4 xa = getPt(P1, P2, settings.currentTime);
////    // glm::vec4 xb = getPt(P2, P3, settings.currentTime);
////    // glm::vec4 xc = getPt(P3, P4, settings.currentTime);
////
////    // // Calculate points on the lines between the above points
////    // glm::vec4 xm = getPt(xa, xb, settings.currentTime);
////    // glm::vec4 xn = getPt(xb, xc, settings.currentTime);
////
////    // // Calculate the final point on the Bezier curve
////    // glm::vec4 pointOnCurve = getPt(xm, xn, settings.currentTime);
////    // m_metaData.cameraData.pos = pointOnCurve;
////
////    settings.xy += 4.f;
////    if (m_controlPointIndex % 1 == 0) {
////        settings.xz += 8.f;
////    }
////    if (m_controlPointIndex % 3 == 0){
////        settings.yz += 8.f;
////    }
//    this->render(data, rtScene);
//
//    QImage flippedImage = image.mirrored(false, false);
//    flippedImage = flippedImage.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    imageLabel->setPixmap(QPixmap::fromImage(flippedImage));
//    // m_controlPointIndex++;
//
//
//    // QTimer::singleShot(3500, this, [this, imageLabel]() {
//    //     // This code will be executed after a 2-second delay
//    //     emit rotationChanged(settings.rotation);
//    // });
//    m_image = image;
//}

void RayTracer::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
    std::cout << "key pressed" << std::endl;

    // J and L for xy rotation
    if (m_keyMap[Qt::Key_J]) {
        std::cout << "key 1" << std::endl;
            settings.xy += settings.rotation;
        emit xyRotationChanged(settings.xy);
    }
    if (m_keyMap[Qt::Key_L]) {
        settings.xy -= settings.rotation;
        emit xyRotationChanged(settings.xy);
    }

    // I and M for xz rotation
    if (m_keyMap[Qt::Key_I]) {
        settings.xz += settings.rotation;
        emit xzRotationChanged(settings.xz);
    }
    if (m_keyMap[Qt::Key_M]) {
        settings.xz -= settings.rotation;
        emit xzRotationChanged(settings.xz);
    }

    // O and N for yz rotation
    if (m_keyMap[Qt::Key_O]) {
        settings.yz += settings.rotation;
        emit yzRotationChanged(settings.yz);
    }
    if (m_keyMap[Qt::Key_N]) {
        settings.yz -= settings.rotation;
        emit yzRotationChanged(settings.yz);
    }

    // W and S for x translation
    if (m_keyMap[Qt::Key_W]) {
        settings.xw += settings.translation;
        emit xwRotationChanged(settings.xw);
    }
    if (m_keyMap[Qt::Key_S]) {
        settings.xw -= settings.translation;
        emit xwRotationChanged(settings.xw);
    }

    // A and D for y translation
    if (m_keyMap[Qt::Key_A]) {
        settings.yw += settings.translation;
        emit yzRotationChanged(settings.yw);
    }
    if (m_keyMap[Qt::Key_D]) {
        settings.yw -= settings.translation;
        emit ywRotationChanged(settings.yw);
    }

    // TODO: add slider for z translation
    // T and G for z translation
    if (m_keyMap[Qt::Key_T]) {
        settings.z += settings.translation;
        emit zChanged(settings.zw);
    }

    if (m_keyMap[Qt::Key_G]) {
        settings.z -= settings.translation;
        emit zChanged(settings.zw);
    }

    // R & F for w translation using zw
    if (m_keyMap[Qt::Key_R]) {
        settings.zw += settings.translation;
        emit zwRotationChanged(settings.zw);
    }
    if (m_keyMap[Qt::Key_F]) {
        settings.zw -= settings.translation;
        emit zwRotationChanged(settings.zw);
    }

    // TODO: ONLY IF HAVE TIME, NOT NEEDED
    // Space & V for vorex depth
    if (m_keyMap[Qt::Key_Space]) {
        settings.w += settings.rotation;

    }
    if (m_keyMap[Qt::Key_V]) {
        settings.w -= settings.rotation;
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
    std::string command = "ffmpeg -framerate 24 -pattern_type glob -i '" + directory + "*.png' -c:v libx264 -pix_fmt yuv420p -crf 0 -y '" + directory + "video.mp4'";
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to assemble video." << std::endl;
    }
}
