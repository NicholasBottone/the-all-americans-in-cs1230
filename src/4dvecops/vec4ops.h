//
// Created by Michael Foiani on 12/8/23.
//

#ifndef PROJECTS_RAY_VEC4OPS_H
#define PROJECTS_RAY_VEC4OPS_H

#include <glm/glm.hpp>

class Vec4Ops {
public:

    static glm::mat4 getRotationMatrix4XY(float angleRadians);

    static glm::mat4 getRotationMatrix4YZ(float angleRadians);

    static glm::mat4 getRotationMatrix4ZX(float angleRadians);

    static glm::mat4 getRotationMatrix4XW(float angleRadians);

    static glm::mat4 getRotationMatrix4YW(float angleRadians);

    static glm::mat4 getRotationMatrix4ZW(float angleRadians);

    glm::vec4 transformPoint4(glm::vec4 point4, glm::mat4 transformDirectionMatrix, glm::vec4 translationPointVector);

    glm::vec4 inverseTransformPoint4(glm::vec4 point4, glm::mat4 inverseTransformDirectionMatrix,
                                     glm::vec4 inverseTranslationPointVector);
};

#endif //PROJECTS_RAY_VEC4OPS_H
