#include "vec4ops.h"

// this is used to transform a 4d point by a 4d matrix and its associated translation
// this is motivated by the fact that glm does not support 5d matrices, so we cannot define a mat5 to encapsulate both the rotation and translation in one matrix
// therefore, we break the 5d transformation into a 4d rotation and a 4d translation
glm::vec4 Vec4Ops::transformPoint4(glm::vec4 point4,  glm::mat4 transformDirectionMatrix, glm::vec4 translationPointVector) {
    // do the translation then direction
    point4 = transformDirectionMatrix * point4;
    point4 += translationPointVector;
    return point4;
}

glm::vec4 Vec4Ops::inverseTransformPoint4(glm::vec4 point4,  glm::mat4 inverseTransformDirectionMatrix, glm::vec4 inverseTranslationPointVector) {
    // do the direction then translation
    point4 += inverseTranslationPointVector;
    point4 = inverseTranslationPointVector * point4;
    return point4;
}