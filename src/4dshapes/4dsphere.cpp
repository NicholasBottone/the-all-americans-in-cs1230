#include "raytracer/raytracer.h"

// vector operations on 4d vectors,
// reference: https://hollasch.github.io/ray4/Four-Space_Visualization_of_4D_Objects.html#chapter5

bool HitSphere(RenderShapeData object, glm::vec4 rayOrigin, glm::vec4 rayDirection, glm::vec4 inter) {
    float bb;
    glm::vec4 cdir;
    float rad;
    float t1, t2;

    cdir = object.ctm * glm::vec4(0.f,0.f,0.f,1.f) - rayOrigin;
    bb = glm::dot(cdir, rayDirection);
    rad = (bb * bb) - glm::dot(cdir, cdir) + 0.25f;

    if (rad < 0.0) {
        return false;
    }

    rad = sqrt(rad);
    t2 = bb - rad;
    t1 = bb + rad;

    if ((t1 < 0.0) || ((t2 > 0.0) && (t2 < t1))) {
        t1 = t2;
    }

    if (t1 <= 0.0) {
        return false;
    }

    if (inter != glm::vec4(0.f,0.f,0.f, 1.f)) {
        inter = rayOrigin + t1 * rayDirection;
    }

    return true;
}