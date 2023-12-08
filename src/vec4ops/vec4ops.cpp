#include <stdexcept>
#include "vec4ops.h"
#include "settings.h"

// vector operations on 4d vectors,
// reference: https://hollasch.github.io/ray4/Four-Space_Visualization_of_4D_Objects.html#chapter5

glm::vec4 Vec4Ops::cross4(
        glm::vec4 u,
        glm::vec4 v,
        glm::vec4 w) {
    float a = (v[0] * w[1]) - (v[1] * w[0]);
    float b = (v[0] * w[2]) - (v[2] * w[0]);
    float c = (v[0] * w[3]) - (v[3] * w[0]);
    float d = (v[1] * w[2]) - (v[2] * w[1]);
    float e = (v[1] * w[3]) - (v[3] * w[1]);
    float f = (v[2] * w[3]) - (v[3] * w[2]);

    glm::vec4 result;
    result[0] = (u[1] * f) - (u[2] * e) + (u[3] * d);
    result[1] = -(u[0] * f) + (u[2] * c) - (u[3] * b);
    result[2] = (u[0] * e) - (u[1] * c) + (u[3] * a);
    result[3] = -(u[0] * d) + (u[1] * b) - (u[2] * a);

    return result;
}

glm::vec4 Vec4Ops::dot4(
        glm::vec4 u,
        glm::vec4 v) {
    return {u[0] * v[0], u[1] * v[1], u[2] * v[2], u[3] * v[3]};
}

glm::mat4 Vec4Ops::getViewMatrix4(
        glm::vec4 fromPoint,
        glm::vec4 toPoint,
        glm::vec4 upVector,
        glm::vec4 lookVector) {

    // rotation matrices for each plane
    glm::mat4 rotMatrixXY = getRotationMatrix4XY(glm::radians(settings.xy));
    glm::mat4 rotMatrixXZ = getRotationMatrix4XZ(glm::radians(settings.xz));
    glm::mat4 rotMatrixYZ = getRotationMatrix4YZ(glm::radians(settings.yz));
    glm::mat4 rotMatrixXW = getRotationMatrix4XW(glm::radians(settings.xw));
    glm::mat4 rotMatrixYW = getRotationMatrix4YW(glm::radians(settings.yw));
    glm::mat4 rotMatrixZW = getRotationMatrix4ZW(glm::radians(settings.zw));

    glm::mat4 combinedRotationMatrix = rotMatrixXY * rotMatrixYZ * rotMatrixXZ * rotMatrixXW * rotMatrixYW * rotMatrixZW;

    // calculate e3 basis vector, the transformation col of view matrix
    if (glm::distance(fromPoint, toPoint) < 0.0001f) {
        throw std::runtime_error("fromPoint and toPoint are the same");
    }
    glm::vec4 e3 = glm::normalize(fromPoint - toPoint);

    // calculate e2 basis vector, from the combinatory cross of up and over with e3
    glm::vec4 e2 = cross4(upVector, lookVector, e3);
    e2 = glm::normalize(e2);
    if (glm::distance(e2, glm::vec4{0, 0, 0, 0}) < 0.0001f) {
        throw std::runtime_error("invalid up vector");
    }

    // calculate e1 basis vector, from the cross of only the over vector
    glm::vec4 e1 = cross4(lookVector, e3, e2);
    e1 = glm::normalize(e1);
    if (glm::distance(e1, glm::vec4{0, 0, 0, 0}) < 0.0001f) {
        throw std::runtime_error("invalid over vector");
    }

    // calculate e0 basis vector, the 4d orthogonal vector to the other 3 bases
    glm::vec4 e0 = cross4(e3, e2, e1);
    e0 = glm::normalize(e0);

    // Apply the combined rotation matrix to the view basis vectors
    e0 = combinedRotationMatrix * e0;
    e1 = combinedRotationMatrix * e1;
    e2 = combinedRotationMatrix * e2;
    e3 = combinedRotationMatrix * e3;

    // Normalizing might be necessary after applying the rotation
    e0 = glm::normalize(e0);
    e1 = glm::normalize(e1);
    e2 = glm::normalize(e2);
    e3 = glm::normalize(e3);

    return {e2, e1, e0, e3};
}