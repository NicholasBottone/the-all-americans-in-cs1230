#include "sceneparser.h"
#include "scenefilereader.h"
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
void initTree(SceneNode* currentNode, std::vector<RenderShapeData> *shapes, std::vector<SceneLightData> *lights, glm::mat4 currentCTM) {
    for (auto t : currentNode->transformations) {
        switch (t->type)
        {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            currentCTM *= glm::translate(glm::vec3(t->translate[0], t->translate[1], t->translate[2]));
            break;
        case TransformationType::TRANSFORMATION_SCALE:
            currentCTM *= glm::scale(glm::vec3(t->scale[0], t->scale[1], t->scale[2]));
            break;
        case TransformationType::TRANSFORMATION_ROTATE:
            currentCTM *= glm::rotate(t->angle, glm::vec3(t->rotate[0], t->rotate[1], t->rotate[2]));
            break;
        case TransformationType::TRANSFORMATION_MATRIX:
            currentCTM *= glm::mat4(t->matrix);
            break;
        default:
            std::cout << "Invalid transformation type" << std::endl;
            break;
        }
    }


    for(auto primitive : currentNode->primitives) {
        // primitive->material.textureData = loadTextureFromFile(QString::fromStdString(primitive->material.textureMap.filename));
        RenderShapeData rsd = {*primitive, currentCTM, glm::inverse(currentCTM)};
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
        initTree(child, shapes, lights, currentCTM);
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

    initTree(root, &renderData.shapes, &renderData.lights, currentCTM);

    return true;
}
