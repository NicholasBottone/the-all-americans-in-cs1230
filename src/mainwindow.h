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
    void connectxy();
    void connectxz();
    void connectxw();
    void connectyz();
    void connectyw();
    void connectzw();

    RayTracer *rayTracer;
    AspectRatioWidget *aspectRatioWidget;
    QPushButton *uploadFile;
    QPushButton *saveImage;
    QSlider *xySlider;
    QDoubleSpinBox *xyBox;
    QSlider *xzSlider;
    QDoubleSpinBox *xzBox;
    QSlider *xwSlider;
    QDoubleSpinBox *xwBox;
    QSlider *yzSlider;
    QDoubleSpinBox *yzBox;
    QSlider *ywSlider;
    QDoubleSpinBox *ywBox;
    QSlider *zwSlider;
    QDoubleSpinBox *zwBox;

private slots:
    void onUploadFile();
    void onSaveImage();
    void onValChangexySlider(int newValue);
    void onValChangexyBox(double newValue);
    void onValChangexzSlider(int newValue);
    void onValChangexzBox(double newValue);
    void onValChangexwSlider(int newValue);
    void onValChangexwBox(double newValue);
    void onValChangeyzSlider(int newValue);
    void onValChangeyzBox(double newValue);
    void onValChangeywSlider(int newValue);
    void onValChangeywBox(double newValue);
    void onValChangezwSlider(int newValue);
    void onValChangezwBox(double newValue);
};
