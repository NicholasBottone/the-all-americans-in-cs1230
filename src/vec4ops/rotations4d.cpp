#include "raytracer/raytracer.h"
#include "vec4ops/vec4ops.h"

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

glm::mat4 Vec4Ops::getRotationMatrix4(
        std::vector<float> anglesRadians)
{
    // make the identity
    glm::mat4 result = glm::mat4(0.f);
    result[0][0] = 1.f;
    result[1][1] = 1.f;
    result[2][2] = 1.f;
    result[3][3] = 1.f;

    // apply the rotations, starting with the last angle, which corresponds to ZW
    if (anglesRadians.size() != 6) {
        throw std::runtime_error("invalid rotation angle array");
    }
    for (int i = 5; i >= 0; i--) {
        switch (i) {
            case 0:
                result *= getRotationMatrix4XY(anglesRadians[i]) * result;
                break;
            case 1:
                result *= getRotationMatrix4ZX(anglesRadians[i]) * result;
                break;
            case 2:
                result *= getRotationMatrix4YZ(anglesRadians[i]) * result;
                break;
            case 3:
                result *= getRotationMatrix4XW(anglesRadians[i]) * result;
                break;
            case 4:
                result *= getRotationMatrix4YW(anglesRadians[i]) * result;
                break;
            case 5:
                result *= getRotationMatrix4ZW(anglesRadians[i]) * result;
                break;
            default:
                throw std::runtime_error("invalid rotation matrix");
        }
    }

    return result;
}

