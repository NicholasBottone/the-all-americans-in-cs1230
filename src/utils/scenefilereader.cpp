#include "scenefilereader.h"
#include "scenedata.h"

#include "glm/gtc/type_ptr.hpp"

#include <cassert>
#include <cstring>
#include <iostream>
#include <filesystem>

#include <QFile>
#include <QJsonArray>

#define ERROR_AT(e) "error at line " << e.lineNumber() << " col " << e.columnNumber() << ": "
#define PARSE_ERROR(e) std::cout << ERROR_AT(e) << "could not parse <" << e.tagName().toStdString() \
                                 << ">" << std::endl
#define UNSUPPORTED_ELEMENT(e) std::cout << ERROR_AT(e) << "unsupported element <" \
                                         << e.tagName().toStdString() << ">" << std::endl;

// Students, please ignore this file.
ScenefileReader::ScenefileReader(const std::string &name) {
    file_name = name;

    memset(&m_cameraData, 0, sizeof(SceneCameraData));
    memset(&m_globalData, 0, sizeof(SceneGlobalData));

    m_root = new SceneNode;

    m_templates.clear();
    m_nodes.clear();

    m_nodes.push_back(m_root);
}

ScenefileReader::~ScenefileReader() {
    // Delete all Scene Nodes
    for (unsigned int node = 0; node < m_nodes.size(); node++) {
        for (size_t i = 0; i < (m_nodes[node])->transformations.size(); i++)
        {
            delete (m_nodes[node])->transformations[i];
        }
        for (size_t i = 0; i < (m_nodes[node])->primitives.size(); i++)
        {
            delete (m_nodes[node])->primitives[i];
        }
        (m_nodes[node])->transformations.clear();
        (m_nodes[node])->primitives.clear();
        (m_nodes[node])->children.clear();
        delete m_nodes[node];
    }

    m_nodes.clear();
    m_templates.clear();
}

SceneGlobalData ScenefileReader::getGlobalData() const {
    return m_globalData;
}

SceneCameraData ScenefileReader::getCameraData() const {
    return m_cameraData;
}

SceneNode *ScenefileReader::getRootNode() const {
    return m_root;
}

// This is where it all goes down...
bool ScenefileReader::readJSON() {
    // Read the file
    QFile file(file_name.c_str());
    if (!file.open(QFile::ReadOnly)) {
        std::cout << "could not open " << file_name << std::endl;
        return false;
    }

    // Load the JSON document
    QByteArray fileContents = file.readAll();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(fileContents, &jsonError);
    if (doc.isNull()) {
        std::cout << "could not parse " << file_name << std::endl;
        std::cout << "parse error at line " << jsonError.offset << ": "
                  << jsonError.errorString().toStdString() << std::endl;
        return false;
    }
    file.close();

    if (!doc.isObject()) {
        std::cout << "document is not an object" << std::endl;
        return false;
    }

    // Get the root element
    QJsonObject scenefile = doc.object();

    if (!scenefile.contains("globalData")) {
        std::cout << "missing required field \"globalData\" on root object" << std::endl;
        return false;
    }
    if (!scenefile.contains("cameraData")) {
        std::cout << "missing required field \"cameraData\" on root object" << std::endl;
        return false;
    }

    QStringList requiredFields = {"globalData", "cameraData"};
    QStringList optionalFields = {"name", "groups", "templateGroups"};
    // If other fields are present, raise an error
    QStringList allFields = requiredFields + optionalFields;
    for (auto &field : scenefile.keys()) {
        if (!allFields.contains(field)) {
            std::cout << "unknown field \"" << field.toStdString() << "\" on root object" << std::endl;
            return false;
        }
    }

    // Parse the global data
    if (!parseGlobalData(scenefile["globalData"].toObject())) {
        std::cout << "could not parse \"globalData\"" << std::endl;
        return false;
    }

    // Parse the camera data
    if (!parseCameraData(scenefile["cameraData"].toObject())) {
        std::cout << "could not parse \"cameraData\"" << std::endl;
        return false;
    }

    // Parse the template groups
    if (scenefile.contains("templateGroups")) {
        if (!parseTemplateGroups(scenefile["templateGroups"])) {
            return false;
        }
    }

    // Parse the groups
    if (scenefile.contains("groups")) {
        if (!parseGroups(scenefile["groups"], m_root)) {
            return false;
        }
    }

    std::cout << "Finished reading " << file_name << std::endl;
    return true;
}

/**
 * Parse a globalData field and fill in m_globalData.
 */
bool ScenefileReader::parseGlobalData(const QJsonObject &globalData) {
    QStringList requiredFields = {"ambientCoeff", "diffuseCoeff", "specularCoeff"};
    QStringList optionalFields = {"transparentCoeff", "gravity"};
    QStringList allFields = requiredFields + optionalFields;
    for (auto field : globalData.keys()) {
        if (!allFields.contains(field)) {
            std::cout << "unknown field \"" << field.toStdString() << "\" on globalData object" << std::endl;
            return false;
        }
    }
    for (auto field : requiredFields) {
        if (!globalData.contains(field)) {
            std::cout << "missing required field \"" << field.toStdString() << "\" on globalData object" << std::endl;
            return false;
        }
    }

    // Parse the global data
    if (globalData["ambientCoeff"].isDouble()) {
        m_globalData.ka = globalData["ambientCoeff"].toDouble();
    }
    else {
        std::cout << "globalData ambientCoeff must be a floating-point value" << std::endl;
        return false;
    }
    if (globalData["diffuseCoeff"].isDouble()) {
        m_globalData.kd = globalData["diffuseCoeff"].toDouble();
    }
    else {
        std::cout << "globalData diffuseCoeff must be a floating-point value" << std::endl;
        return false;
    }
    if (globalData["specularCoeff"].isDouble()) {
        m_globalData.ks = globalData["specularCoeff"].toDouble();
    }
    else {
        std::cout << "globalData specularCoeff must be a floating-point value" << std::endl;
        return false;
    }
    if (globalData.contains("transparentCoeff")) {
        if (globalData["transparentCoeff"].isDouble()) {
            m_globalData.kt = globalData["transparentCoeff"].toDouble();
        }
        else {
            std::cout << "globalData transparentCoeff must be a floating-point value" << std::endl;
            return false;
        }
    }
    if (globalData.contains("gravity")) {
        if (globalData["gravity"].isArray()) {
            QJsonArray gravityArray = globalData["gravity"].toArray();
            if (gravityArray.size() != 3 && gravityArray.size() != 4) {
                std::cout << "globalData gravity must have 3-4 elements" << std::endl;
                return false;
            }
            if (!gravityArray[0].isDouble() || !gravityArray[1].isDouble() || !gravityArray[2].isDouble() || (gravityArray.size() == 4 && !gravityArray[3].isDouble())) {
                std::cout << "globalData gravity must contain floating-point values" << std::endl;
                return false;
            }
            m_globalData.gravity.x = gravityArray[0].toDouble();
            m_globalData.gravity.y = gravityArray[1].toDouble();
            m_globalData.gravity.z = gravityArray[2].toDouble();
            if (gravityArray.size() == 4) {
                m_globalData.gravity.w = gravityArray[3].toDouble();
            }
            else {
                m_globalData.gravity.w = 1.f;
            }
        }
        else {
            std::cout << "globalData gravity must be an array" << std::endl;
            return false;
        }
    }

    return true;
}

/**
 * Parse a Light and add a new CS123SceneLightData to m_lights.
 */
bool ScenefileReader::parseLightData(const QJsonObject &lightData, SceneNode *node) {
    QStringList requiredFields = {"type", "color"};
    QStringList optionalFields = {"name", "attenuationCoeff", "direction", "penumbra", "angle", "width", "height"};
    QStringList allFields = requiredFields + optionalFields;
    for (auto &field : lightData.keys()) {
        if (!allFields.contains(field)) {
            std::cout << "unknown field \"" << field.toStdString() << "\" on light object" << std::endl;
            return false;
        }
    }
    for (auto &field : requiredFields) {
        if (!lightData.contains(field)) {
            std::cout << "missing required field \"" << field.toStdString() << "\" on light object" << std::endl;
            return false;
        }
    }

    // Create a default light
    SceneLight *light = new SceneLight();
    memset(light, 0, sizeof(SceneLight));
    node->lights.push_back(light);

    light->dir = glm::vec4(0.f, 0.f, 0.f, 0.f);
    light->function = glm::vec3(1, 0, 0);

    // parse the color
    if (!lightData["color"].isArray()) {
        std::cout << "light color must be of type array" << std::endl;
        return false;
    }
    QJsonArray colorArray = lightData["color"].toArray();
    if (colorArray.size() != 3) {
        std::cout << "light color must be of size 3" << std::endl;
        return false;
    }
    if (!colorArray[0].isDouble() || !colorArray[1].isDouble() || !colorArray[2].isDouble()) {
        std::cout << "light color must contain floating-point values" << std::endl;
        return false;
    }
    light->color.r = colorArray[0].toDouble();
    light->color.g = colorArray[1].toDouble();
    light->color.b = colorArray[2].toDouble();

    // parse the type
    if (!lightData["type"].isString()) {
        std::cout << "light type must be of type string" << std::endl;
        return false;
    }
    std::string lightType = lightData["type"].toString().toStdString();

    // parse directional light
    if (lightType == "directional") {
        light->type = LightType::LIGHT_DIRECTIONAL;

        // parse direction
        if (!lightData.contains("direction")) {
            std::cout << "directional light must contain field \"direction\"" << std::endl;
            return false;
        }
        if (!lightData["direction"].isArray()) {
            std::cout << "directional light direction must be of type array" << std::endl;
            return false;
        }
        QJsonArray directionArray = lightData["direction"].toArray();
        if (directionArray.size() != 3) {
            std::cout << "directional light direction must be of size 3" << std::endl;
            return false;
        }
        if (!directionArray[0].isDouble() || !directionArray[1].isDouble() || !directionArray[2].isDouble()) {
            std::cout << "directional light direction must contain floating-point values" << std::endl;
            return false;
        }
        light->dir.x = directionArray[0].toDouble();
        light->dir.y = directionArray[1].toDouble();
        light->dir.z = directionArray[2].toDouble();
    }
    else if (lightType == "point") {
        light->type = LightType::LIGHT_POINT;

        // parse the attenuation coefficient
        if (!lightData.contains("attenuationCoeff")) {
            std::cout << "point light must contain field \"attenuationCoeff\"" << std::endl;
            return false;
        }
        if (!lightData["attenuationCoeff"].isArray()) {
            std::cout << "point light attenuationCoeff must be of type array" << std::endl;
            return false;
        }
        QJsonArray attenuationArray = lightData["attenuationCoeff"].toArray();
        if (attenuationArray.size() != 3) {
            std::cout << "point light attenuationCoeff must be of size 3" << std::endl;
            return false;
        }
        if (!attenuationArray[0].isDouble() || !attenuationArray[1].isDouble() || !attenuationArray[2].isDouble()) {
            std::cout << "point light attenuationCoeff must contain floating-point values" << std::endl;
            return false;
        }
        light->function.x = attenuationArray[0].toDouble();
        light->function.y = attenuationArray[1].toDouble();
        light->function.z = attenuationArray[2].toDouble();
    }
    else if (lightType == "spot") {
        QStringList pointRequiredFields = {"direction", "penumbra", "angle", "attenuationCoeff"};
        for (auto &field : pointRequiredFields) {
            if (!lightData.contains(field)) {
                std::cout << "missing required field \"" << field.toStdString() << "\" on spotlight object" << std::endl;
                return false;
            }
        }
        light->type = LightType::LIGHT_SPOT;

        // parse direction
        if (!lightData["direction"].isArray()) {
            std::cout << "spotlight direction must be of type array" << std::endl;
            return false;
        }
        QJsonArray directionArray = lightData["direction"].toArray();
        if (directionArray.size() != 3) {
            std::cout << "spotlight direction must be of size 3" << std::endl;
            return false;
        }
        if (!directionArray[0].isDouble() || !directionArray[1].isDouble() || !directionArray[2].isDouble()) {
            std::cout << "spotlight direction must contain floating-point values" << std::endl;
            return false;
        }
        light->dir.x = directionArray[0].toDouble();
        light->dir.y = directionArray[1].toDouble();
        light->dir.z = directionArray[2].toDouble();

        // parse attenuation coefficient
        if (!lightData["attenuationCoeff"].isArray()) {
            std::cout << "spotlight attenuationCoeff must be of type array" << std::endl;
            return false;
        }
        QJsonArray attenuationArray = lightData["attenuationCoeff"].toArray();
        if (attenuationArray.size() != 3) {
            std::cout << "spotlight attenuationCoeff must be of size 3" << std::endl;
            return false;
        }
        if (!attenuationArray[0].isDouble() || !attenuationArray[1].isDouble() || !attenuationArray[2].isDouble()) {
            std::cout << "spotlight direction must contain floating-point values" << std::endl;
            return false;
        }
        light->function.x = attenuationArray[0].toDouble();
        light->function.y = attenuationArray[1].toDouble();
        light->function.z = attenuationArray[2].toDouble();

        // parse penumbra
        if (!lightData["penumbra"].isDouble()) {
            std::cout << "spotlight penumbra must be of type float" << std::endl;
            return false;
        }
        light->penumbra = lightData["penumbra"].toDouble() * M_PI / 180.f;

        // parse angle
        if (!lightData["angle"].isDouble()) {
            std::cout << "spotlight angle must be of type float" << std::endl;
            return false;
        }
        light->angle = lightData["angle"].toDouble() * M_PI / 180.f;
    }
    else if (lightType == "area") {
        light->type = LightType::LIGHT_AREA;

        QStringList pointRequiredFields = {"width", "height"};
        for (auto &field : pointRequiredFields) {
            if (!lightData.contains(field)) {
                std::cout << "missing required field \"" << field.toStdString() << "\" on area light object" << std::endl;
                return false;
            }
        }

        // parse width
        if (!lightData["width"].isDouble()) {
            std::cout << "area light penumbra must be of type float" << std::endl;
            return false;
        }
        light->width = lightData["width"].toDouble();

        // parse height
        if (!lightData["height"].isDouble()) {
            std::cout << "area light height must be of type float" << std::endl;
            return false;
        }
        light->height = lightData["height"].toDouble();

        // parse the attenuation coefficient
        if (!lightData.contains("attenuationCoeff")) {
            std::cout << "area light must contain field \"attenuationCoeff\"" << std::endl;
            return false;
        }
        if (!lightData["attenuationCoeff"].isArray()) {
            std::cout << "area light attenuationCoeff must be of type array" << std::endl;
            return false;
        }
        QJsonArray attenuationArray = lightData["attenuationCoeff"].toArray();
        if (attenuationArray.size() != 3) {
            std::cout << "area light attenuationCoeff must be of size 3" << std::endl;
            return false;
        }
        if (!attenuationArray[0].isDouble() || !attenuationArray[1].isDouble() || !attenuationArray[2].isDouble()) {
            std::cout << "area light attenuationCoeff must contain floating-point values" << std::endl;
            return false;
        }
        light->function.x = attenuationArray[0].toDouble();
        light->function.y = attenuationArray[1].toDouble();
        light->function.z = attenuationArray[2].toDouble();
    }
    else {
        std::cout << "unknown light type \"" << lightType << "\"" << std::endl;
        return false;
    }

    return true;
}

/**
 * Parse cameraData and fill in m_cameraData.
 */
bool ScenefileReader::parseCameraData(const QJsonObject &cameraData) {
    QStringList requiredFields = {"position", "up", "heightAngle"};
    QStringList optionalFields = {"aperture", "focalLength", "look", "focus", "depthAngle", "over"};
    QStringList allFields = requiredFields + optionalFields;
    for (auto &field : cameraData.keys()) {
        if (!allFields.contains(field)) {
            std::cout << "unknown field \"" << field.toStdString() << "\" on cameraData object" << std::endl;
            return false;
        }
    }
    for (auto &field : requiredFields) {
        if (!cameraData.contains(field)) {
            std::cout << "missing required field \"" << field.toStdString() << "\" on cameraData object" << std::endl;
            return false;
        }
    }

    // Must have either look or focus, but not both
    if (cameraData.contains("look") && cameraData.contains("focus")) {
        std::cout << "cameraData cannot contain both \"look\" and \"focus\"" << std::endl;
        return false;
    }

    // Parse the camera data
    if (cameraData["position"].isArray()) {
        QJsonArray position = cameraData["position"].toArray();
        if (position.size() != 3 && position.size() != 4) {
            std::cout << "cameraData position must have 3-4 elements" << std::endl;
            return false;
        }
        if (!position[0].isDouble() || !position[1].isDouble() || !position[2].isDouble() || (position.size() == 4 && !position[3].isDouble())) {
            std::cout << "cameraData position must be a floating-point value" << std::endl;
            return false;
        }
        m_cameraData.pos.x = position[0].toDouble();
        m_cameraData.pos.y = position[1].toDouble();
        m_cameraData.pos.z = position[2].toDouble();
        if (position.size() == 4) {
            m_cameraData.pos.w = position[3].toDouble();
        }
        else {
            m_cameraData.pos.w = 1.f;
        }
    }
    else {
        std::cout << "cameraData position must be an array" << std::endl;
        return false;
    }

    if (cameraData["up"].isArray()) {
        QJsonArray up = cameraData["up"].toArray();
        if (up.size() != 3 && up.size() != 4) {
            std::cout << "cameraData up must have 3-4 elements" << std::endl;
            return false;
        }
        if (!up[0].isDouble() || !up[1].isDouble() || !up[2].isDouble() || (up.size() == 4 && !up[3].isDouble())) {
            std::cout << "cameraData up must be a floating-point value" << std::endl;
            return false;
        }
        m_cameraData.up.x = up[0].toDouble();
        m_cameraData.up.y = up[1].toDouble();
        m_cameraData.up.z = up[2].toDouble();
        if (up.size() == 4) {
            m_cameraData.up.w = up[3].toDouble();
        }
        else {
            m_cameraData.up.w = 1.f;
        }
    }
    else {
        std::cout << "cameraData up must be an array" << std::endl;
        return false;
    }

    if (cameraData["heightAngle"].isDouble()) {
        m_cameraData.heightAngle = cameraData["heightAngle"].toDouble() * M_PI / 180.f;
    }
    else {
        std::cout << "cameraData heightAngle must be a floating-point value" << std::endl;
        return false;
    }

    if (cameraData.contains("aperture")) {
        if (cameraData["aperture"].isDouble()) {
            m_cameraData.aperture = cameraData["aperture"].toDouble();
        }
        else {
            std::cout << "cameraData aperture must be a floating-point value" << std::endl;
            return false;
        }
    }

    if (cameraData.contains("focalLength")) {
        if (cameraData["focalLength"].isDouble()) {
            m_cameraData.focalLength = cameraData["focalLength"].toDouble();
        }
        else {
            std::cout << "cameraData focalLength must be a floating-point value" << std::endl;
            return false;
        }
    }

    // Parse the look or focus
    // if the focus is specified, we will convert it to a look vector later
    if (cameraData.contains("look")) {
        if (cameraData["look"].isArray()) {
            QJsonArray look = cameraData["look"].toArray();
            if (look.size() != 3 && look.size() != 4) {
                std::cout << "cameraData look must have 3-4 elements" << std::endl;
                return false;
            }
            if (!look[0].isDouble() || !look[1].isDouble() || !look[2].isDouble() || (look.size() == 4 && !look[3].isDouble())) {
                std::cout << "cameraData look must be a floating-point value" << std::endl;
                return false;
            }
            m_cameraData.look.x = look[0].toDouble();
            m_cameraData.look.y = look[1].toDouble();
            m_cameraData.look.z = look[2].toDouble();
            if (look.size() == 4) {
                m_cameraData.look.w = look[3].toDouble();
            }
            else {
                m_cameraData.look.w = 1.f;
            }
        }
        else {
            std::cout << "cameraData look must be an array" << std::endl;
            return false;
        }
    }
    else if (cameraData.contains("focus")) {
        if (cameraData["focus"].isArray()) {
            QJsonArray focus = cameraData["focus"].toArray();
            if (focus.size() != 3 && focus.size() != 4) {
                std::cout << "cameraData focus must have 3-4 elements" << std::endl;
                return false;
            }
            if (!focus[0].isDouble() || !focus[1].isDouble() || !focus[2].isDouble() || (focus.size() == 4 && !focus[3].isDouble())) {
                std::cout << "cameraData focus must be a floating-point value" << std::endl;
                return false;
            }
            m_cameraData.look.x = focus[0].toDouble();
            m_cameraData.look.y = focus[1].toDouble();
            m_cameraData.look.z = focus[2].toDouble();
            if (focus.size() == 4) {
                m_cameraData.look.w = focus[3].toDouble();
            }
            else {
                m_cameraData.look.w = 1.f;
            }
        }
        else {
            std::cout << "cameraData focus must be an array" << std::endl;
            return false;
        }
    }

    if (cameraData.contains("depthAngle")) {
        if (cameraData["depthAngle"].isDouble()) {
            m_cameraData.depthAngle = cameraData["depthAngle"].toDouble() * M_PI / 180.f;
        }
        else {
            std::cout << "cameraData depthAngle must be a floating-point value" << std::endl;
            return false;
        }
    }

    if (cameraData.contains("over") && cameraData["over"].isArray()) {
        QJsonArray over = cameraData["over"].toArray();
        if (over.size() != 3 && over.size() != 4) {
            std::cout << "cameraData over must have 3-4 elements" << std::endl;
            return false;
        }
        if (!over[0].isDouble() || !over[1].isDouble() || !over[2].isDouble() || (over.size() == 4 && !over[3].isDouble())) {
            std::cout << "cameraData over must be a floating-point value" << std::endl;
            return false;
        }
        m_cameraData.over.x = over[0].toDouble();
        m_cameraData.over.y = over[1].toDouble();
        m_cameraData.over.z = over[2].toDouble();
        if (over.size() == 4) {
            m_cameraData.over.w = over[3].toDouble();
        }
        else {
            m_cameraData.over.w = 1.f;
        }
    }

    // Convert the focus point (stored in the look vector) into a
    // look vector from the camera position to that focus point.
    if (cameraData.contains("focus")) {
        m_cameraData.look -= m_cameraData.pos;
    }

    return true;
}

bool ScenefileReader::parseTemplateGroups(const QJsonValue &templateGroups) {
    if (!templateGroups.isArray()) {
        std::cout << "templateGroups must be an array" << std::endl;
        return false;
    }

    QJsonArray templateGroupsArray = templateGroups.toArray();
    for (auto templateGroup : templateGroupsArray) {
        if (!templateGroup.isObject()) {
            std::cout << "templateGroup items must be of type object" << std::endl;
            return false;
        }

        if (!parseTemplateGroupData(templateGroup.toObject())) {
            return false;
        }
    }

    return true;
}

bool ScenefileReader::parseTemplateGroupData(const QJsonObject &templateGroup) {
    QStringList requiredFields = {"name"};
    QStringList optionalFields = {"translate", "rotate", "scale", "matrix", "lights", "primitives", "groups"};
    QStringList allFields = requiredFields + optionalFields;
    for (auto &field : templateGroup.keys()) {
        if (!allFields.contains(field)) {
            std::cout << "unknown field \"" << field.toStdString() << "\" on templateGroup object" << std::endl;
            return false;
        }
    }

    for (auto &field : requiredFields) {
        if (!templateGroup.contains(field)) {
            std::cout << "missing required field \"" << field.toStdString() << "\" on templateGroup object" << std::endl;
            return false;
        }
    }

    if (!templateGroup["name"].isString()) {
        std::cout << "templateGroup name must be a string" << std::endl;
    }
    if (m_templates.contains(templateGroup["name"].toString().toStdString())) {
        std::cout << "templateGroups cannot have the same" << std::endl;
    }

    SceneNode *templateNode = new SceneNode;
    m_nodes.push_back(templateNode);
    m_templates[templateGroup["name"].toString().toStdString()] = templateNode;

    return parseGroupData(templateGroup, templateNode);
}

/**
 * Parse a group object and create a new CS123SceneNode in m_nodes.
 * NAME OF NODE CANNOT REFERENCE TEMPLATE NODE
 */
bool ScenefileReader::parseGroupData(const QJsonObject &object, SceneNode *node) {
    QStringList optionalFields = {"name", "translate", "rotate", "scale", "matrix", "lights", "primitives", "groups"};
    QStringList allFields = optionalFields;
    for (auto &field : object.keys()) {
        if (!allFields.contains(field)) {
            std::cout << "unknown field \"" << field.toStdString() << "\" on group object" << std::endl;
            return false;
        }
    }

    // parse translation if defined
    if (object.contains("translate")) {
        if (!object["translate"].isArray()) {
            std::cout << "group translate must be of type array" << std::endl;
            return false;
        }

        QJsonArray translateArray = object["translate"].toArray();
        if (translateArray.size() != 4 && translateArray.size() != 3) {
            std::cout << "group translate must have 3-4 elements" << std::endl;
            return false;
        }
        if (!translateArray[0].isDouble() || !translateArray[1].isDouble() || !translateArray[2].isDouble() || (translateArray.size() == 4 && !translateArray[3].isDouble())) {
            std::cout << "group translate must contain floating-point values" << std::endl;
            return false;
        }

        SceneTransformation *translation = new SceneTransformation();
        translation->type = TransformationType::TRANSFORMATION_TRANSLATE;
        translation->translate.x = translateArray[0].toDouble();
        translation->translate.y = translateArray[1].toDouble();
        translation->translate.z = translateArray[2].toDouble();
        if (translateArray.size() == 4)
            translation->translate.w = translateArray[3].toDouble();

        node->transformations.push_back(translation);
    }

    // parse rotation if defined
    if (object.contains("rotate")) {
        if (!object["rotate"].isArray()) {
            std::cout << "group rotate must be of type array" << std::endl;
            return false;
        }

        QJsonArray rotateArray = object["rotate"].toArray();
        if (rotateArray.size() != 7 && rotateArray.size() != 4) {
            std::cout << "group rotate must have 4 or 7 elements" << std::endl;
            return false;
        }
        if (!rotateArray[0].isDouble() || !rotateArray[1].isDouble() || !rotateArray[2].isDouble() || !rotateArray[3].isDouble() || (rotateArray.size() == 7 && (!rotateArray[4].isDouble() || !rotateArray[5].isDouble() || !rotateArray[6].isDouble()))) {
            std::cout << "group rotate must contain floating-point values" << std::endl;
            return false;
        }

        SceneTransformation *rotation = new SceneTransformation();
        rotation->type = TransformationType::TRANSFORMATION_ROTATE;
        rotation->rotate3.x = rotateArray[0].toDouble();
        rotation->rotate3.y = rotateArray[1].toDouble();
        rotation->rotate3.z = rotateArray[2].toDouble();
        if (rotateArray.size() == 7) {
            rotation->rotateW.x = rotateArray[4].toDouble();
            rotation->rotateW.y = rotateArray[5].toDouble();
            rotation->rotateW.z = rotateArray[6].toDouble();
        } else {
            rotation->rotateW.x = 0;
            rotation->rotateW.y = 0;
            rotation->rotateW.z = 0;
        }
        rotation->angle = rotateArray[(rotateArray.size() == 7) ? 3 : 6].toDouble() * M_PI / 180.f;

        node->transformations.push_back(rotation);
    }

    // parse scale if defined
    if (object.contains("scale")) {
        if (!object["scale"].isArray()) {
            std::cout << "group scale must be of type array" << std::endl;
            return false;
        }

        QJsonArray scaleArray = object["scale"].toArray();
        if (scaleArray.size() != 4 && scaleArray.size() != 3) {
            std::cout << "group scale must have 3-4 elements" << std::endl;
            return false;
        }
        if (!scaleArray[0].isDouble() || !scaleArray[1].isDouble() || !scaleArray[2].isDouble() || (scaleArray.size() == 4 && !scaleArray[3].isDouble())) {
            std::cout << "group scale must contain floating-point values" << std::endl;
            return false;
        }

        SceneTransformation *scale = new SceneTransformation();
        scale->type = TransformationType::TRANSFORMATION_SCALE;
        scale->scale.x = scaleArray[0].toDouble();
        scale->scale.y = scaleArray[1].toDouble();
        scale->scale.z = scaleArray[2].toDouble();
        if (scaleArray.size() == 4)
            scale->scale.w = scaleArray[3].toDouble();

        node->transformations.push_back(scale);
    }

    // parse matrix if defined
    if (object.contains("matrix")) {
        if (!object["matrix"].isArray()) {
            std::cout << "group matrix must be of type array of array" << std::endl;
            return false;
        }

        QJsonArray matrixArray = object["matrix"].toArray();
        if (matrixArray.size() != 5 && matrixArray.size() != 4) {
            std::cout << "group matrix must be 4x4 or 5x5" << std::endl;
            return false;
        }

        SceneTransformation *matrixTransformation = new SceneTransformation();
        matrixTransformation->type = TransformationType::TRANSFORMATION_MATRIX;

        float *matrixPtr = glm::value_ptr(matrixTransformation->matrix);
        float *matrixRightPtr = glm::value_ptr(matrixTransformation->matrixRight4d);
        int rowIndex = 0;
        for (auto row : matrixArray) {
            if (rowIndex == 4) break; // skip the last row

            if (!row.isArray()) {
                std::cout << "group matrix must be of type array of array" << std::endl;
                return false;
            }

            QJsonArray rowArray = row.toArray();
            if (rowArray.size() != 5 && rowArray.size() != 4) {
                std::cout << "group matrix must be 4x4 or 5x5" << std::endl;
                return false;
            }

            int colIndex = 0;
            for (auto val : rowArray) {
                if (!val.isDouble()) {
                    std::cout << "group matrix must contain all floating-point values" << std::endl;
                    return false;
                }

                // fill in column-wise
                if (colIndex == 4) {
                    matrixRightPtr[rowIndex] = (float)val.toDouble();
                } else {
                    matrixPtr[colIndex * 4 + rowIndex] = (float)val.toDouble();
                }
                colIndex++;
            }
            rowIndex++;
        }

        node->transformations.push_back(matrixTransformation);
    }

    // parse lights if any
    if (object.contains("lights")) {
        if (!object["lights"].isArray()) {
            std::cout << "group lights must be of type array" << std::endl;
            return false;
        }
        QJsonArray lightsArray = object["lights"].toArray();
        for (auto light : lightsArray) {
            if (!light.isObject()) {
                std::cout << "light must be of type object" << std::endl;
                return false;
            }

            if (!parseLightData(light.toObject(), node)) {
                return false;
            }
        }
    }

    // parse primitives if any
    if (object.contains("primitives")) {
        if (!object["primitives"].isArray()) {
            std::cout << "group primitives must be of type array" << std::endl;
            return false;
        }
        QJsonArray primitivesArray = object["primitives"].toArray();
        for (auto primitive : primitivesArray) {
            if (!primitive.isObject()) {
                std::cout << "primitive must be of type object" << std::endl;
                return false;
            }

            if (!parsePrimitive(primitive.toObject(), node)) {
                return false;
            }
        }
    }

    // parse children groups if any
    if (object.contains("groups")) {
        if (!parseGroups(object["groups"], node)) {
            return false;
        }
    }

    return true;
}

bool ScenefileReader::parseGroups(const QJsonValue &groups, SceneNode *parent) {
    if (!groups.isArray()) {
        std::cout << "groups must be of type array" << std::endl;
        return false;
    }

    QJsonArray groupsArray = groups.toArray();
    for (auto group : groupsArray) {
        if (!group.isObject()) {
            std::cout << "group items must be of type object" << std::endl;
            return false;
        }

        QJsonObject groupData = group.toObject();
        if (groupData.contains("name")) {
            if (!groupData["name"].isString()) {
                std::cout << "group name must be of type string" << std::endl;
                return false;
            }

            // if its a reference to a template group append it
            std::string groupName = groupData["name"].toString().toStdString();
            if (m_templates.contains(groupName)) {
                parent->children.push_back(m_templates[groupName]);
                continue;
            }
        }

        SceneNode *node = new SceneNode;
        m_nodes.push_back(node);
        parent->children.push_back(node);

        if (!parseGroupData(group.toObject(), node)) {
            return false;
        }
    }

    return true;
}

/**
 * Parse an <object type="primitive"> tag into node.
 */
bool ScenefileReader::parsePrimitive(const QJsonObject &prim, SceneNode *node) {
    QStringList requiredFields = {"type"};
    QStringList optionalFields = {
        "meshFile", "ambient", "diffuse", "specular", "reflective", "transparent", "shininess", "ior",
        "blend", "textureFile", "textureU", "textureV", "bumpMapFile", "bumpMapU", "bumpMapV"};

    QStringList allFields = requiredFields + optionalFields;
    for (auto field : prim.keys()) {
        if (!allFields.contains(field)) {
            std::cout << "unknown field \"" << field.toStdString() << "\" on primitive object" << std::endl;
            return false;
        }
    }
    for (auto field : requiredFields) {
        if (!prim.contains(field)) {
            std::cout << "missing required field \"" << field.toStdString() << "\" on primitive object" << std::endl;
            return false;
        }
    }

    if (!prim["type"].isString()) {
        std::cout << "primitive type must be of type string" << std::endl;
        return false;
    }
    std::string primType = prim["type"].toString().toStdString();

    // Default primitive
    ScenePrimitive *primitive = new ScenePrimitive();
    SceneMaterial &mat = primitive->material;
    mat.clear();
    primitive->type = PrimitiveType::PRIMITIVE_CUBE;
    mat.textureMap.isUsed = false;
    mat.bumpMap.isUsed = false;
    mat.cDiffuse.r = mat.cDiffuse.g = mat.cDiffuse.b = 1;
    node->primitives.push_back(primitive);

    std::filesystem::path basePath = std::filesystem::path(file_name).parent_path().parent_path();
    if (primType == "sphere")
        primitive->type = PrimitiveType::PRIMITIVE_SPHERE;
    else if (primType == "cube")
        primitive->type = PrimitiveType::PRIMITIVE_CUBE;
    else if (primType == "cylinder")
        primitive->type = PrimitiveType::PRIMITIVE_CYLINDER;
    else if (primType == "cone")
        primitive->type = PrimitiveType::PRIMITIVE_CONE;
    else if (primType == "mesh") {
        primitive->type = PrimitiveType::PRIMITIVE_MESH;
        if (!prim.contains("meshFile")) {
            std::cout << "primitive type mesh must contain field meshFile" << std::endl;
            return false;
        }
        if (!prim["meshFile"].isString()) {
            std::cout << "primitive meshFile must be of type string" << std::endl;
            return false;
        }

        std::filesystem::path relativePath(prim["meshFile"].toString().toStdString());
        primitive->meshfile = (basePath / relativePath).string();
    }
    else {
        std::cout << "unknown primitive type \"" << primType << "\"" << std::endl;
        return false;
    }

    if (prim.contains("ambient")) {
        if (!prim["ambient"].isArray()) {
            std::cout << "primitive ambient must be of type array" << std::endl;
            return false;
        }
        QJsonArray ambientArray = prim["ambient"].toArray();
        if (ambientArray.size() != 3) {
            std::cout << "primitive ambient array must be of size 3" << std::endl;
            return false;
        }

        for (int i = 0; i < 3; i++) {
            if (!ambientArray[i].isDouble()) {
                std::cout << "primitive ambient must contain floating-point values" << std::endl;
                return false;
            }

            mat.cAmbient[i] = ambientArray[i].toDouble();
        }
    }

    if (prim.contains("diffuse")) {
        if (!prim["diffuse"].isArray()) {
            std::cout << "primitive diffuse must be of type array" << std::endl;
            return false;
        }
        QJsonArray diffuseArray = prim["diffuse"].toArray();
        if (diffuseArray.size() != 3) {
            std::cout << "primitive diffuse array must be of size 3" << std::endl;
            return false;
        }

        for (int i = 0; i < 3; i++) {
            if (!diffuseArray[i].isDouble()) {
                std::cout << "primitive diffuse must contain floating-point values" << std::endl;
                return false;
            }

            mat.cDiffuse[i] = diffuseArray[i].toDouble();
        }
    }

    if (prim.contains("specular")) {
        if (!prim["specular"].isArray()) {
            std::cout << "primitive specular must be of type array" << std::endl;
            return false;
        }
        QJsonArray specularArray = prim["specular"].toArray();
        if (specularArray.size() != 3) {
            std::cout << "primitive specular array must be of size 3" << std::endl;
            return false;
        }

        for (int i = 0; i < 3; i++) {
            if (!specularArray[i].isDouble()) {
                std::cout << "primitive specular must contain floating-point values" << std::endl;
                return false;
            }

            mat.cSpecular[i] = specularArray[i].toDouble();
        }
    }

    if (prim.contains("reflective")) {
        if (!prim["reflective"].isArray()) {
            std::cout << "primitive reflective must be of type array" << std::endl;
            return false;
        }
        QJsonArray reflectiveArray = prim["reflective"].toArray();
        if (reflectiveArray.size() != 3) {
            std::cout << "primitive reflective array must be of size 3" << std::endl;
            return false;
        }

        for (int i = 0; i < 3; i++) {
            if (!reflectiveArray[i].isDouble()) {
                std::cout << "primitive reflective must contain floating-point values" << std::endl;
                return false;
            }

            mat.cReflective[i] = reflectiveArray[i].toDouble();
        }
    }

    if (prim.contains("transparent")) {
        if (!prim["transparent"].isArray()) {
            std::cout << "primitive transparent must be of type array" << std::endl;
            return false;
        }
        QJsonArray transparentArray = prim["transparent"].toArray();
        if (transparentArray.size() != 3) {
            std::cout << "primitive transparent array must be of size 3" << std::endl;
            return false;
        }

        for (int i = 0; i < 3; i++) {
            if (!transparentArray[i].isDouble()) {
                std::cout << "primitive transparent must contain floating-point values" << std::endl;
                return false;
            }

            mat.cTransparent[i] = transparentArray[i].toDouble();
        }
    }

    if (prim.contains("shininess")) {
        if (!prim["shininess"].isDouble()) {
            std::cout << "primitive shininess must be of type float" << std::endl;
            return false;
        }

        mat.shininess = (float) prim["shininess"].toDouble();
    }

    if (prim.contains("ior")) {
        if (!prim["ior"].isDouble()) {
            std::cout << "primitive ior must be of type float" << std::endl;
            return false;
        }

        mat.ior = (float) prim["ior"].toDouble();
    }

    if (prim.contains("blend")) {
        if (!prim["blend"].isDouble()) {
            std::cout << "primitive blend must be of type float" << std::endl;
            return false;
        }

        mat.blend = (float)prim["blend"].toDouble();
    }

    if (prim.contains("textureFile")) {
        if (!prim["textureFile"].isString()) {
            std::cout << "primitive textureFile must be of type string" << std::endl;
            return false;
        }
        std::filesystem::path fileRelativePath(prim["textureFile"].toString().toStdString());

        mat.textureMap.filename = (basePath / fileRelativePath).string();
        mat.textureMap.repeatU = prim.contains("textureU") && prim["textureU"].isDouble() ? prim["textureU"].toDouble() : 1;
        mat.textureMap.repeatV = prim.contains("textureV") && prim["textureV"].isDouble() ? prim["textureV"].toDouble() : 1;
        mat.textureMap.isUsed = true;
    }

    if (prim.contains("bumpMapFile")) {
        if (!prim["bumpMapFile"].isString()) {
            std::cout << "primitive bumpMapFile must be of type string" << std::endl;
            return false;
        }
        std::filesystem::path fileRelativePath(prim["bumpMapFile"].toString().toStdString());

        mat.bumpMap.filename = (basePath / fileRelativePath).string();
        mat.bumpMap.repeatU = prim.contains("bumpMapU") && prim["bumpMapU"].isDouble() ? prim["bumpMapU"].toDouble() : 1;
        mat.bumpMap.repeatV = prim.contains("bumpMapV") && prim["bumpMapV"].isDouble() ? prim["bumpMapV"].toDouble() : 1;
        mat.bumpMap.isUsed = true;
    }

    if (prim.contains("velocity")) {
        if (!prim["velocity"].isArray()) {
            std::cout << "primitive velocity must be of type array" << std::endl;
            return false;
        }
        QJsonArray velocityArray = prim["velocity"].toArray();
        if (velocityArray.size() != 3 && velocityArray.size() != 4) {
            std::cout << "primitive velocity array must be of size 3-4" << std::endl;
            return false;
        }

        for (int i = 0; i < velocityArray.size(); i++) {
            if (!velocityArray[i].isDouble()) {
                std::cout << "primitive velocity must contain floating-point values" << std::endl;
                return false;
            }

            primitive->velocity[i] = velocityArray[i].toDouble();
        }
    }

    return true;
}
