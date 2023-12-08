#include "raytracer/raytracer.h"

// this is used to transform a 4d point by a 4d matrix and its associated translation
// this is motivated by the fact that glm does not support 5d matrices, so we cannot define a mat5 to encapsulate both the rotation and translation in one matrix
// therefore, we break the 5d transformation into a 4d rotation and a 4d translation
glm::vec4 transformPoint4(glm::vec4 point4,  glm::mat4 transformDirectionMatrix, glm::vec4 translationPointVector) {
    // do the translation first
    point4 -= translationPointVector;
    // do the rotation and scaling
    point4 = transformDirectionMatrix * point4;
    return point4;
}