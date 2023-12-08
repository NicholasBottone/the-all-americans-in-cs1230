#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
// #include "realtime.h"
#include "raytracer/raytracer.h"
#include "utils/aspectratiowidget/aspectratiowidget.hpp"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    void initialize();
    void finish();
    QLabel *imageLabel;

private:
    void connectUIElements();
    void connectUploadFile();
    void connectSaveImage();
    void connectW();

    RayTracer *rayTracer;
    AspectRatioWidget *aspectRatioWidget;
    QPushButton *uploadFile;
    QPushButton *saveImage;
    QSlider *wSlider;
    QDoubleSpinBox *wBox;

private slots:
    void onUploadFile();
    void onSaveImage();
    void onValChangeWSlider(int newValue);
    void onValChangeWBox(double newValue);
};
