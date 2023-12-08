#pragma once

#include "scenedata.h"
#include <vector>
#include <string>
#include "rgba.h"

// Struct which contains data for a single primitive, to be used for rendering
struct RenderShapeData {
    ScenePrimitive primitive;
    glm::mat4 ctm; // the cumulative transformation matrix
    glm::vec4 translation4d; // appended to the right of the ctm (for 4d translation)
    glm::mat4 inverseCTM;
    glm::vec4 inverseTranslation4d; // appended to the right of the inverse ctm (for 4d translation)
};

// Struct which contains all the data needed to render a scene
struct RenderData {
    SceneGlobalData globalData;
    SceneCameraData cameraData;

    std::vector<SceneLightData> lights;
    std::vector<RenderShapeData> shapes;
};

class SceneParser {
public:
    // Parse the scene and store the results in renderData.
    // @param filepath    The path of the scene file to load.
    // @param renderData  On return, this will contain the metadata of the loaded scene.
    // @return            A boolean value indicating whether the parse was successful.
    static bool parse(std::string filepath, RenderData &renderData);

    static glm::mat4 getRotationMatrix4(float angle, glm::vec3 axis3, glm::vec3 axisW);

    static void translate4(glm::vec4 &v1, glm::vec4 v2);

    static void scale4(glm::vec4 &v1, glm::vec4 v2);
};
