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
};

#endif //PROJECTS_RAY_VEC4OPS_H
