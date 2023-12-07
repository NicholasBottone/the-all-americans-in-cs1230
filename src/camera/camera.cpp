#include <stdexcept>
#include "camera.h"

Camera::Camera(SceneCameraData cameraData) :
    m_pos(cameraData.pos),
    m_heightAngle(cameraData.heightAngle),
    m_focalLength(cameraData.focalLength),
    m_aperture(cameraData.aperture)
{
    // VIEW MATRIX INTIALIZATION
    // cast to 3 for dots & cross
    glm::vec3 look3{cameraData.look.x, cameraData.look.y, cameraData.look.z};
    glm::vec3 up3{cameraData.up.x, cameraData.up.y, cameraData.up.z};

    // calculate new basis
    glm::vec3 e0 = -glm::normalize(look3);
    glm::vec3 e1 = glm::normalize(up3 - glm::dot(up3, e0) * e0);
    glm::vec3 e2 = glm::cross(e1, e0);

    glm::mat4 alignment
    {
        e2.x, e1.x, e0.x, 0.f,
        e2.y, e1.y, e0.y, 0.f,
        e2.z, e1.z, e0.z, 0.f,
        0.f, 0.f, 0.f, 1.f
    };
    glm::mat4 translation
    {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        -cameraData.pos.x, -cameraData.pos.y, -cameraData.pos.z, 1.f
    };

    m_viewMatrix = alignment * translation;
    m_inverse = glm::inverse(m_viewMatrix);
}


glm::mat4 Camera::getViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return m_viewMatrix;
}

glm::mat4 Camera::getInverseViewMatrix() const {
    // Optional TODO: implement the getter or make your own design
    return m_inverse;
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

glm::vec3 Camera::getPos() const {
    return m_pos;
}
