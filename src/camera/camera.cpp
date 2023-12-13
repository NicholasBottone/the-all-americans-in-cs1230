#include <stdexcept>
#include "camera.h"
#include "vec4ops/vec4ops.h"
#include "settings.h"

Camera::Camera(SceneCameraData cameraData) :
    m_pos(cameraData.pos),
    m_heightAngle(cameraData.heightAngle),
    m_focalLength(cameraData.focalLength),
    m_aperture(cameraData.aperture)
{
     m_viewMatrix = Vec4Ops::getViewMatrix4(cameraData.look, cameraData.up, cameraData.over);
     m_translationVector = -cameraData.pos;

     m_inverseViewMatrix = glm::inverse(m_viewMatrix);
     m_inverseTranslationVector = -m_translationVector;
}


glm::mat4 Camera::getViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return m_viewMatrix;
}

glm::mat4 Camera::getInverseViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return m_inverseViewMatrix;
}

glm::vec4 Camera::getTranslationVector() const {
    return m_translationVector;
}

glm::vec4 Camera::getInverseTranslationVector() const {
    return m_inverseTranslationVector;
}

float Camera::getAspectRatio() const {
    // Optional TODO: implement the getter or make your own design
    throw std::runtime_error("not implemented");
}

float Camera::getHeightAngle() const {
    // Optional TODO: implement the getter or make your own design
    return m_heightAngle;
}

float Camera::getFocalLength() const {
    // Optional TODO: implement the getter or make your own design
    return m_focalLength;
}

float Camera::getAperture() const {
    // Optional TODO: implement the getter or make your own design
    return m_aperture;
}