#include <QList>
#include <QtConcurrent>
#include <iostream>
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
}

// updated to use 4D
void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    if (m_enableParallelism) {
        renderParallel(imageData, scene);
    } else {
        // naive rendering
        Camera camera = scene.getCamera();
        float cameraDepth = 1.f;

        float viewplaneHeight = 2.f*cameraDepth*std::tan(camera.getHeightAngle() / 2.f);
        float viewplaneWidth = cameraDepth*viewplaneHeight*((float)scene.width()/(float)scene.height());

        for (int imageRow = 0; imageRow < scene.height(); imageRow++) {
            for (int imageCol = 0; imageCol < scene.width(); imageCol++) {
                // FIXME: for now, use height as depth
                for (int imageDepth = 0; imageDepth < scene.height(); imageDepth++) {
                    // compute the ray
                    float x = (imageCol - scene.width()/2.f) * viewplaneWidth / scene.width();
                    float y = (imageRow - scene.height()/2.f) * viewplaneHeight / scene.height();
                    float z = (imageDepth - scene.height()/2.f) * viewplaneHeight / scene.height();
                    float camera4dDepth = 1;

                    glm::vec4 pWorld = Vec4Ops::transformPoint4(glm::vec4(x, y, z, 0.f), camera.getViewMatrix(), camera.getTranslationVector());
                    glm::vec4 dWorld = glm::vec4(0.f, 0.f, 0.f, -1.f);

                    // get the pixel color
                    glm::vec4 pixelColor = getPixelFromRay(pWorld, dWorld, scene, 0);

                    // set the pixel color
                    int index = imageRow * scene.width() + imageCol;
                    imageData[index] = RGBA{
                        (std::uint8_t) (pixelColor.r * 255.f),
                        (std::uint8_t) (pixelColor.g * 255.f),
                        (std::uint8_t) (pixelColor.b * 255.f),
                        (std::uint8_t) (pixelColor.a * 255.f)
                    };
                }
            }
        }
    }

    if (settings.bulkOutputFolderPath.size() > 0) { // means we are doing bulk rendering
        // save the image to the bulk directory
        std::string filePath = settings.bulkOutputFolderPath + QDir::separator().toLatin1() + std::to_string(settings.currentTime) + ".png";
        saveViewportImage(filePath);
        if (settings.currentTime < settings.maxTime) { // still more to render
            // render the next frame
            settings.currentTime++;
            emit settingsChanged(m_imageLabel); // emit to allow the UI to update then render the next frame
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
        m_image.fill(Qt::black);
        imageLabel->setPixmap(QPixmap::fromImage(m_image));
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
    bool success = SceneParser::parse(settings.sceneFilePath, m_metaData);

    if (!success) {
        std::cerr << "Error loading scene: \"" << settings.sceneFilePath << "\"" << std::endl;
        // return;
        // render a blank image
        QImage image = QImage(576, 432, QImage::Format_RGBX8888);
        image.fill(Qt::black);
        RGBA *data = reinterpret_cast<RGBA *>(image.bits());
        m_imageData = data;
        imageLabel->setPixmap(QPixmap::fromImage(image));
        return;
    }

    int width = 576;
    int height = 432;

    QImage image = QImage(width, height, QImage::Format_RGBX8888);
    image.fill(Qt::black);
    RGBA *data = reinterpret_cast<RGBA *>(image.bits());

    RayTraceScene rtScene{ width, height, m_metaData };
    this->render(data, rtScene);

    QImage flippedImage = image.mirrored(false, false);
    flippedImage = flippedImage.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(QPixmap::fromImage(flippedImage));
    // QTimer::singleShot(3500, this, [this, imageLabel]() {
    //     // This code will be executed after a 2-second delay
    //     emit rotationChanged(settings.rotation);
    // });
    m_image = image;
}

glm::vec4 getPt(glm::vec3 n1 , glm::vec3 n2 , float perc )
{
    glm::vec3 diff = n2 - n1;

    return glm::vec4(n1 + ( diff * perc ), 0.0f);
} 

void RayTracer::wSliderChanged(QLabel* imageLabel) { 
    // auto P1 = cameraControlPoints[0];
    // auto P2 = scene.getCamera().controlPoints[1];
    // auto P3 = scene.getCamera().controlPoints[2];
    // auto P4 = scene.getCamera().controlPoints[3];
    // for( float i = 0 ; i < 1 ; i += 0.01 )
    // {
    //     glm::vec3 xa = getPt(P1, P2, i);
    //     glm::vec3 xb = getPt(P2, P3, i);
    //     glm::vec3 xc = getPt(P3, P4, i);

    //     // Calculate points on the lines between the above points
    //     glm::vec3 xm = getPt(xa, xb, i);
    //     glm::vec3 xn = getPt(xb, xc, i);

    //     // Calculate the final point on the Bezier curve
    //     glm::vec3 pointOnCurve = getPt(xm, xn, i);

    //     // update the camera position
    //     m_metaData.cameraData.pos = pointOnCurve;
    //     RayTraceScene rtScene{ m_width, m_height, m_metaData, m_depth };
    //     this->render(m_imageData, rtScene);
    //     QTimer::singleShot(3500, this, [this, imageLabel]() {
    //         // This code will be executed after a 2-second delay
    //         // emit rotationChanged(settings.rotation);
    //         QImage flippedImage = m_image.mirrored(false, false);
    //         flippedImage = flippedImage.scaled(m_width, m_height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //         imageLabel->setPixmap(QPixmap::fromImage(flippedImage));
    //     });
    // }

    // emit cameraPositionChanged(m_metaData.cameraData.pos);
    
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
