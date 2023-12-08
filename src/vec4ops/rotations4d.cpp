#include "raytracer/raytracer.h"
#include "4dvecops/vec4ops.h"

glm::mat4 Vec4Ops::getRotationMatrix4XY(
        float angleRadians) {
    glm::mat4 result;
    result[0][0] = cos(angleRadians);
    result[0][1] = -sin(angleRadians);
    result[1][0] = sin(angleRadians);
    result[1][1] = cos(angleRadians);
    result[2][2] = 1;
    result[3][3] = 1;
    return result;
}

glm::mat4 Vec4Ops::getRotationMatrix4YZ(
        float angleRadians) {
    glm::mat4 result;
    result[1][1] = cos(angleRadians);
    result[1][2] = -sin(angleRadians);
    result[2][1] = sin(angleRadians);
    result[2][2] = cos(angleRadians);
    result[0][0] = 1;
    result[3][3] = 1;
    return result;
}

glm::mat4 Vec4Ops::getRotationMatrix4ZX(
        float angleRadians) {
    glm::mat4 result;
    result[2][2] = cos(angleRadians);
    result[2][0] = -sin(angleRadians);
    result[0][2] = sin(angleRadians);
    result[0][0] = cos(angleRadians);
    result[1][1] = 1;
    result[3][3] = 1;
    return result;
}

glm::mat4 Vec4Ops::getRotationMatrix4XW(
        float angleRadians) {
    glm::mat4 result;
    result[0][0] = cos(angleRadians);
    result[0][3] = -sin(angleRadians);
    result[3][0] = sin(angleRadians);
    result[3][3] = cos(angleRadians);
    result[1][1] = 1;
    result[2][2] = 1;
    return result;
}

glm::mat4 Vec4Ops::getRotationMatrix4YW(
        float angleRadians) {
    glm::mat4 result;
    result[1][1] = cos(angleRadians);
    result[1][3] = -sin(angleRadians);
    result[3][1] = sin(angleRadians);
    result[3][3] = cos(angleRadians);
    result[0][0] = 1;
    result[2][2] = 1;
    return result;
}

glm::mat4 Vec4Ops::getRotationMatrix4ZW(
        float angleRadians) {
    glm::mat4 result;
    result[2][2] = cos(angleRadians);
    result[2][3] = -sin(angleRadians);
    result[3][2] = sin(angleRadians);
    result[3][3] = cos(angleRadians);
    result[0][0] = 1;
    result[1][1] = 1;
    return result;
}