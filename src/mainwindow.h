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
    void connectBulkRender();
    void connectxy();
    void connectxz();
    void connectxw();
    void connectyz();
    void connectyw();
    void connectzw();
    void connectMaxTimeSlider();
    void connectNegativeRotation();
    void connectW();
    void connectz();

    RayTracer *rayTracer;
    AspectRatioWidget *aspectRatioWidget;
    QPushButton *uploadFile;
    QPushButton *saveImage;
    QPushButton *bulkRender;
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
    QSlider *maxTimeSlider;
    QSpinBox *maxTimeSpinBox;
    QPushButton *rotateNegative;
    QSlider *wSlider;
    QDoubleSpinBox *wBox;
    QLabel *currTimeLabel;
    QSlider *zSlider;
    QDoubleSpinBox *zBox;


private slots:
    void onUploadFile();
    void onSaveImage();
    void onBulkRender();
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
    void onValChangeMaxTimeSlider(int newValue);
    void onValChangeMaxTimeBox(double newValue);
    void onRotateNegative();
    void updateXySlider(double newValue);
    void updateXzSlider(double newValue);
    void updateXwSlider(double newValue);
    void updateYzSlider(double newValue);
    void updateYwSlider(double newValue);
    void updateZwSlider(double newValue);
    void updateRotationSlider(float newValue);
    void onValChangeWSlider(int newValue);
    void onValChangeWBox(double newValue);
    // void updateCameraPosition();
    void updateTimeValue();

    void onValChangezSlider(int newValue);
    void onValChangezBox(double newValue);
    void updateZSlider(double newValue);

};
