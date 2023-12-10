#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
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
    void connectRotationSlider();
    void connectNegativeRotation();
    void connectW();

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
    QSlider *rotationSlider;
    QDoubleSpinBox *rotationBox;
    QCheckBox *rotateNegative;
    QSlider *wSlider;
    QDoubleSpinBox *wBox;


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
    void onValChangeRotationSlider(int newValue);
    void onValChangeRotationBox(double newValue);
    void onRotateNegative();
    void updateXySlider(double newValue);
    void updateXzSlider(double newValue);
    void updateXwSlider(double newValue);
    void updateYzSlider(double newValue);
    void updateYwSlider(double newValue);
    void updateZwSlider(double newValue);
    void onValChangeWSlider(int newValue);
    void onValChangeWBox(double newValue);
};
