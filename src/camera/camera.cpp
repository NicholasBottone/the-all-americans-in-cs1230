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
     m_controlPoints = {
        {cameraData.pos[0], cameraData.pos[1], cameraData.pos[2]},
        {cameraData.pos[0], cameraData.pos[1] - 2.f, cameraData.pos[2] - 2.f},
        {cameraData.pos[0] + 2.f, cameraData.pos[1] + 2.f, cameraData.pos[2] -2.f},
        {cameraData.pos[0] + 2.f, cameraData.pos[1], cameraData.pos[2]}
     };
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

// int getPt(glm::vec3 n1 , glm::vec3 n2 , float perc )
// {
//     int diff = n2 - n1;

//     return n1 + ( diff * perc );
// }  

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

    
// }