#include "sceneparser.h"
#include "scenefilereader.h"
#include "vec4ops/vec4ops.h"
#include <glm/gtx/transform.hpp>
#include <QImage>
#include <iostream>
#include "../settings.h"


/**
 * @brief Stores the image specified from the input file in this class's
 * `std::vector<RGBA> m_image`.
 * @param file: file path to an image
 * @return True if successfully loads image, False otherwise.
 */
TextureData loadTextureFromFile(const QString &file) {
    QImage myTexture;

    int width; int height;
    if (!myTexture.load(file)) {
        std::cout<<"Failed to load in image: " << file.toStdString() << std::endl;
        return TextureData{0, 0, nullptr};
    }
    myTexture = myTexture.convertToFormat(QImage::Format_RGBX8888);
    width = myTexture.width();
    height = myTexture.height();

    RGBA* texture = new RGBA[width*height];
    QByteArray arr = QByteArray::fromRawData((const char*) myTexture.bits(), myTexture.sizeInBytes());

    for (int i = 0; i < arr.size() / 4.f; i++){
        texture[i] = RGBA{(std::uint8_t) arr[4*i], (std::uint8_t) arr[4*i+1], (std::uint8_t) arr[4*i+2], (std::uint8_t) arr[4*i+3]};
    }

    return TextureData{width, height, texture};
}

// helper to handle recursive creation of tree
void initTree(SceneNode* currentNode, std::vector<RenderShapeData> *shapes, std::vector<SceneLightData> *lights, glm::mat4 currentCTM, glm::vec4 currentTranslation4d) {
    if (currentNode->transformations.size() == 0) {
        // ScenePa.rser::translate4(currentTranslation4d, glm::vec4(0.f, 0.f, 0.f, settings.w));
        // convert currentTranslation4d to a 4x4 matrix
        currentCTM = glm::translate(glm::mat4(1.0f), glm::vec3(currentTranslation4d));
        currentCTM *= SceneParser::getRotationMatrix4(1.f, glm::vec3(settings.xy, settings.xz, settings.yz), glm::vec3(settings.xw, settings.yw, settings.zw));
    }

    SceneParser::translate4(currentTranslation4d, glm::vec4(0.f, 0.f, 0.f, settings.w));

    for (auto t : currentNode->transformations) {
        switch (t->type)
        {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            SceneParser::translate4(currentTranslation4d, glm::vec4(t->translate.xyz(), 0.f));
            // currentCTM = glm::translate(glm::mat4(1.0f), glm::vec3(currentTranslation4d));
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            currentCTM *= SceneParser::getRotationMatrix4(t->angle, t->rotate3, t->rotateW);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            SceneParser::scale4(currentCTM, t->scale);
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            currentCTM *= glm::mat4(t->matrix);
            currentTranslation4d *= glm::vec4(t->matrixRight4d); // TODO
            break;
        default:
            std::cout << "Invalid transformation type" << std::endl;
            break;
        }
    }

    currentCTM *= glm::translate(glm::mat4(1.0f), glm::vec3(currentTranslation4d));
    

    for(auto primitive : currentNode->primitives) {
        // primitive->material.textureData = loadTextureFromFile(QString::fromStdString(primitive->material.textureMap.filename));
        RenderShapeData rsd = {
            .primitive = *primitive,
            .ctm = currentCTM,
            .translation4d = currentTranslation4d,
            .inverseCTM = glm::inverse(currentCTM),
            .inverseTranslation4d = -currentTranslation4d,
        };
        shapes->push_back(rsd);
    }

    // add the lights
    for(auto l : currentNode->lights) {
        SceneLightData sld{};
        sld.id = l->id;
        sld.color = l->color;
        sld.function = l->function;

        switch (l->type)
        {
        case LightType::LIGHT_POINT:
            sld.type = LightType::LIGHT_POINT;
            sld.pos = currentCTM * glm::vec4(0.f, 0.f, 0.f, 1.f);
            sld.dir = glm::vec4(0.0f);
            break;
        case LightType::LIGHT_DIRECTIONAL:
            sld.type = LightType::LIGHT_DIRECTIONAL;
            sld.pos = glm::vec4(0.0f);
            sld.dir = glm::vec4(currentCTM * l->dir);
            break;
        case LightType::LIGHT_SPOT:
            sld.type = LightType::LIGHT_SPOT;
            sld.pos = currentCTM * glm::vec4(0.f, 0.f, 0.f, 1.f);
            sld.dir = currentCTM * l->dir;
            sld.penumbra = l->penumbra;
            sld.angle = l->angle;
            break;
        case LightType::LIGHT_AREA:
            sld.type = LightType::LIGHT_AREA;
            sld.pos = currentCTM * glm::vec4(0.f, 0.f, 0.f, 1.f);
            sld.width = l->width;
            sld.height = l->height;
            break;
        default:
            std::cout << "Invalid light type" << std::endl;
            continue;
        }

        lights->push_back(sld);
    }

    for (auto child : currentNode->children) {
        initTree(child, shapes, lights, currentCTM, currentTranslation4d);
    }

}


bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }

    // TODO: Use your Lab 5 code here
    // Task 5: populate renderData with global data, and camera data;
    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();

    // Task 6: populate renderData's list of primitives and their transforms.
    //         This will involve traversing the scene graph, and we recommend you
    //         create a helper function to do so!
    SceneNode* root = fileReader.getRootNode();
    renderData.shapes.clear();
    renderData.lights.clear();
    auto currentCTM = glm::mat4(1.0f);
    auto currentTranslation4d = glm::vec4(0.0f);

    initTree(root, &renderData.shapes, &renderData.lights, currentCTM, currentTranslation4d);

    return true;
}

// glm::mat4 SceneParser::getRotationMatrix4(
//         float angle,
//         glm::vec3 axis3,
//         glm::vec3 axisW) {
//     // start with the normal rotation from the normal 3d axes
//     if (axis3.x > 0)
//         return Vec4Ops::getRotationMatrix4XY(angle);
//     else if (axis3.y > 0)
//         return Vec4Ops::getRotationMatrix4YZ(angle);
//     else if (axis3.z > 0)
//         return Vec4Ops::getRotationMatrix4ZX(angle);
//     else if (axisW.x > 0)
//         return Vec4Ops::getRotationMatrix4XW(angle);
//     else if (axisW.y > 0)
//         return Vec4Ops::getRotationMatrix4YW(angle);
//     else if (axisW.z > 0)
//         return Vec4Ops::getRotationMatrix4ZW(angle);
//     else
//         throw std::runtime_error("invalid axis");
// }

glm::mat4 SceneParser::getRotationMatrix4(float angle, glm::vec3 axis3, glm::vec3 axisW) {
    glm::mat4 rotationMatrix3D = glm::mat4(1.0f); // Identity matrix

    // convert axis3 and axisW to radians
    axis3 = glm::radians(axis3);
    axisW = glm::radians(axisW);

    // Apply 3D rotations if any axis is non-zero
    if (axis3.x != 0) rotationMatrix3D *= glm::rotate(axis3.x, glm::vec3(1, 0, 0));
    if (axis3.y != 0) rotationMatrix3D *= glm::rotate(axis3.y, glm::vec3(0, 1, 0));
    if (axis3.z != 0) rotationMatrix3D *= glm::rotate(axis3.z, glm::vec3(0, 0, 1));

    glm::mat4 rotationMatrix4D = glm::mat4(1.0f); // Identity matrix

    // Apply 4D rotations if any axis is non-zero
    if (axisW.x != 0) rotationMatrix4D *= Vec4Ops::getRotationMatrix4XW(axisW.x);
    if (axisW.y != 0) rotationMatrix4D *= Vec4Ops::getRotationMatrix4YW(axisW.y);
    if (axisW.z != 0) rotationMatrix4D *= Vec4Ops::getRotationMatrix4ZW(axisW.z);

    // Combine both 3D and 4D rotations
    return rotationMatrix4D * rotationMatrix3D;
}

void SceneParser::translate4(
        glm::vec4 &v1,
        glm::vec4 v2
        ) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;
}

void SceneParser::scale4(
        glm::mat4 &m,
        glm::vec4 v
        ) {
    m[0][0] *= v.x;
    m[1][1] *= v.y;
    m[2][2] *= v.z;
    m[3][3] *= v.w;
}
