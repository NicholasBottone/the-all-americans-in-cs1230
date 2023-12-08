#include <QCoreApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QtCore>
#include <QApplication>
#include <QScreen>
#include <iostream>
#include <QSettings>
#include <QSurfaceFormat>

#include <iostream>
#include "utils/sceneparser.h"
#include "raytracer/raytracer.h"
#include "raytracer/raytracescene.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName("The All Americans 4D Raytracer");
    QCoreApplication::setOrganizationName("CS 1230/2230");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    QSurfaceFormat fmt;
    fmt.setVersion(4, 1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;
    w.initialize();
    w.resize(800, 600);
    w.show();
    int return_val = a.exec();
    w.finish();
    return return_val;
}
