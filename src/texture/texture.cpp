//
// Created by Michael Foiani on 11/4/23.
//

#include "raytracer/raytracer.h"

glm::vec2 getUVCube(glm::vec4 pObject) {
    float u = -1.f,v = -1.f;

    if (RayTracer::floatEquals(pObject.y, -.5f)) // neg y, bottom face
    {
        u = pObject.x + 0.5f;
        v = pObject.z + 0.5f;
    }
    else if (RayTracer::floatEquals(pObject.y, .5f)) // pos y, top face
    {
        u = pObject.x + 0.5f;
        v = .5f - pObject.z; // flip z
    }
    else if (RayTracer::floatEquals(pObject.x, -.5f)) // neg x, left face
    {
        u = pObject.z + 0.5f;
        v = pObject.y + 0.5f;
    }
    else if (RayTracer::floatEquals(pObject.x, .5f)) // pos x, right face
    {
        u = .5f - pObject.z; // flip z
        v = pObject.y + 0.5f;
    }
    else if (RayTracer::floatEquals(pObject.z, -.5f)) // neg z, back face
    {
        u = .5f - pObject.x; // flip x
        v = pObject.y + 0.5f;
    }
    else if (RayTracer::floatEquals(pObject.z, .5f)) // pos z, front face
    {
        u = pObject.x + 0.5f;
        v = pObject.y + 0.5f;
    }

    return {u, v};
}

glm::vec2 getUVCone(glm::vec4 pObject) {
    float u, v;

    // three cases -> 1) top cap, 2) bottom cap, 3) conical side
    if (RayTracer::floatEquals(pObject.y, -.5f)) // 1) bottom cap
    {
        u = pObject.x + 0.5f;
        v = pObject.z + 0.5f;
    }
    else if (RayTracer::floatEquals(pObject.y, .5f)) // 2) top cap
    {
        u = pObject.x + 0.5f;
        v = 0.5f - pObject.z; // flip z
    }
    else // case 3) conical face
    {
        // get u from theta zpos and xpos
        float theta = glm::atan(pObject.x, pObject.z);
        u = (theta + 1.5 * M_PI) / (2 * M_PI);

        // get v from ypos, trivial
        v = pObject.y + 0.5f;
    }

    return {u, v};
}

glm::vec2 getUVCylinder(glm::vec4 pObject) {
    float u, v;

    // three cases -> top cap, bottom cap, cylindrical side
    if (RayTracer::floatEquals(pObject.y, -.5f)) // 1) bottom cap
    {
        u = pObject.x + 0.5f;
        v = pObject.z + 0.5f;
    }
    else if (RayTracer::floatEquals(pObject.y, .5f)) // 2) top cap
    {
        u = pObject.x + 0.5f;
        v = 0.5f - pObject.z; // flip z
    }
    else // case 3) cylindrical face
    {
        // get u from theta zpos and xpos
        float theta = glm::atan(pObject.x, pObject.z);
        u = (theta + 1.5 * M_PI) / (2 * M_PI);

        // get v from ypos and origin
        v = pObject.y + 0.5f;
    }

    return {u, v};
}

glm::vec2 getUVMesh(glm::vec4 pObject) {
    return glm::vec2(-1.f);
}

glm::vec2 getUVSphere(glm::vec4 pObject) {
    float u = -1.f,v = -1.f;

    // get u from theta between xpos and zpos
    // get u from theta zpos and xpos
    float theta = glm::atan(pObject.x, pObject.z);
    u = (theta + 1.5 * M_PI) / (2 * M_PI);

    // get v from phi from ypos and origin
    float height = pObject.y/.5f;
    height = std::clamp(height, -1.f, 1.f);
    float phi = glm::asin(height);
    v = phi / M_PI + .5f;

    return {u, v};
}

glm::vec4 RayTracer::interpolateTexture(
        glm::vec4 pObject,
        const RenderShapeData &shape,
        glm::vec4 illuminationToInterpolate)
{
    auto material = shape.primitive.material;
    if (!material.textureMap.isUsed)
    {
        // return if no texture
        return illuminationToInterpolate;
    }

    // determine uv based on shape
    glm::vec2 uv;
    switch (shape.primitive.type)
    {
        case PrimitiveType::PRIMITIVE_CUBE:
            uv = getUVCube(pObject);
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            uv = getUVCone(pObject);
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            uv = getUVCylinder(pObject);
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            uv = getUVSphere(pObject);
            break;
        case PrimitiveType::PRIMITIVE_MESH:
            uv = getUVMesh(pObject);
            break;
    }

    float u = uv.x, v = uv.y;
    if (u == -1.f) {
        return illuminationToInterpolate;
    }

    // map u,v to texture image
    TextureData textureData = material.textureData;
    if (textureData.data == nullptr) {
        return illuminationToInterpolate;
    }

    int m = material.textureMap.repeatU;
    int c = (int) glm::floor(u * m * textureData.width) % textureData.width;
    if (c >= textureData.width) {
        c = textureData.width - 1;
    }
    int n = material.textureMap.repeatV;
    int r = (int) glm::floor((1-v) * n * textureData.height) % textureData.height;
    if (r >= textureData.height) {
        r = textureData.height - 1;
    }
    RGBA texture = textureData.data[r * textureData.width + c];

    // interpolate the texture color with the illumination
    float blend = shape.primitive.material.blend;
    glm::vec4 blended = blend * (glm::vec4(texture.r, texture.g, texture.b, texture.a) / 255.f)
                        + (1.f - blend) * illuminationToInterpolate;
    return blended;
}