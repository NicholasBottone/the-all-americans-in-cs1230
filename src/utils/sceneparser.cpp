#include "sceneparser.h"
#include "scenefilereader.h"
#include "4dvecops/vec4ops.h"
#include <glm/gtx/transform.hpp>
#include <QImage>
#include <iostream>


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
    for (auto t : currentNode->transformations) {
        switch (t->type)
        {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            SceneParser::translate4(currentTranslation4d, t->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            SceneParser::scale4(currentTranslation4d, t->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            currentCTM *= SceneParser::getRotationMatrix4(t->angle, t->rotate3, t->rotateW);
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


    for(auto primitive : currentNode->primitives) {
        // primitive->material.textureData = loadTextureFromFile(QString::fromStdString(primitive->material.textureMap.filename));
        RenderShapeData rsd = {
            primitive: *primitive,
            ctm: currentCTM,
            translation4d: currentTranslation4d,
            inverseCTM: glm::inverse(currentCTM),
            inverseTranslation4d: -currentTranslation4d,
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

glm::mat4 SceneParser::getRotationMatrix4(
        float angle,
        glm::vec3 axis3,
        glm::vec3 axisW) {
    // start with the normal rotation from the normal 3d axes
    if (axis3.x > 0)
        return Vec4Ops::getRotationMatrix4XY(angle);
    else if (axis3.y > 0)
        return Vec4Ops::getRotationMatrix4YZ(angle);
    else if (axis3.z > 0)
        return Vec4Ops::getRotationMatrix4ZX(angle);
    else if (axisW.x > 0)
        return Vec4Ops::getRotationMatrix4XW(angle);
    else if (axisW.y > 0)
        return Vec4Ops::getRotationMatrix4YW(angle);
    else if (axisW.z > 0)
        return Vec4Ops::getRotationMatrix4ZW(angle);
    else
        throw std::runtime_error("invalid axis");
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
        glm::vec4 &v1,
        glm::vec4 v2
        ) {
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    v1.w *= v2.w;
}