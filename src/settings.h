#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
    std::string sceneFilePath;
    float xy = 0.f;
    float z = 0.f;
    float xz = 0.f;
    float yz = 0.f;
    float xw = 0.f;
    float yw = 0.f;
    float zw = 0.f;
    float rotation = 0.f;
    bool negative = false;
    float w = 0.f;
    int currentTime = 0;
    int maxTime = 0;
    std::string bulkOutputFilePath;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
