#include <stdexcept>
#include "camera.h"
#include "vec4ops/vec4ops.h"
#include "settings.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>  // Include this header for glm::rotate

void Camera::updateViewMatrix(SceneCameraData cameraData) {
    m_viewMatrix = Vec4Ops::getViewMatrix4(cameraData.look, cameraData.up, cameraData.over);
    m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(settings.xy), glm::vec3(0.f, 1.f, 0.f));
    m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(settings.yz), glm::vec3(1.f, 0.f, 0.f));
    m_viewMatrix = glm::rotate(m_viewMatrix, glm::radians(settings.xz), glm::vec3(0.f, 0.f, 1.f));


    // TODO: link sliders here. make them smaller changes since our objects are so small
    glm::vec4 uiTranslation = glm::vec4(settings.xw, settings.yw, 0.f, settings.zw);
    m_translationVector = -cameraData.pos + uiTranslation;

    m_inverseViewMatrix = glm::inverse(m_viewMatrix);
    m_inverseTranslationVector = -m_translationVector;
    m_controlPoints = {
            {cameraData.pos[0], cameraData.pos[1], cameraData.pos[2]},
            {cameraData.pos[0], cameraData.pos[1] - 2.f, cameraData.pos[2] - 2.f},
            {cameraData.pos[0] + 2.f, cameraData.pos[1] + 2.f, cameraData.pos[2] -2.f},
            {cameraData.pos[0] + 2.f, cameraData.pos[1], cameraData.pos[2]}
    };
}

Camera::Camera(SceneCameraData cameraData) :
    m_pos(cameraData.pos),
    m_heightAngle(cameraData.heightAngle),
    m_focalLength(cameraData.focalLength),
    m_aperture(cameraData.aperture)
{
    updateViewMatrix(cameraData);
}

glm::mat4 Camera::getViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return m_viewMatrix;
}

glm::mat4 Camera::getInverseViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return m_inverseViewMatrix;
}

glm::vec4 Camera::getPos() const {
    // Optional TODO: implement the getter or make your own design
    return m_pos;
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

