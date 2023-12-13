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

    static glm::vec4 transformPoint4(glm::vec4 point4, glm::mat4 transformDirectionMatrix, glm::vec4 translationPointVector);

    static glm::vec4 inverseTransformPoint4(glm::vec4 point4, glm::mat4 inverseTransformDirectionMatrix,
                                     glm::vec4 inverseTranslationPointVector);

    static glm::vec4 cross4(glm::vec4 u, glm::vec4 v, glm::vec4 w);

    static glm::vec4 dot4(glm::vec4 u, glm::vec4 v);

    static glm::mat4 getViewMatrix4(glm::vec4 fromPoint, glm::vec4 toPoint, glm::vec4 upVector, glm::vec4 lookVector);

    static glm::vec4 transformDir4(glm::vec4 dir4, glm::mat4 transformDirectionMatrix);

    static glm::vec4 inverseTransformDir4(glm::vec4 dir4, glm::mat4 inverseTransformDirectionMatrix);
};

#endif //PROJECTS_RAY_VEC4OPS_H
