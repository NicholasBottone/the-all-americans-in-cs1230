#include "raytracer/raytracer.h"

// vector operations on 4d vectors,
// reference: https://hollasch.github.io/ray4/Four-Space_Visualization_of_4D_Objects.html#chapter5

glm::vec4 cross4(
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

glm::vec4 dot4(
        glm::vec4 u,
        glm::vec4 v) {
    return {u[0] * v[0], u[1] * v[1], u[2] * v[2], u[3] * v[3]};
}