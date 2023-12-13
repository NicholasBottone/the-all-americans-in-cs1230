#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSettings>
#include <QLabel>
#include <QGroupBox>
#include <iostream>

void MainWindow::initialize() {
    // create RayTracer
    rayTracer = new RayTracer(this);
    imageLabel = new QLabel(this); 
    // aspectRatioWidget = new AspectRatioWidget(this);
    // aspectRatioWidget->setAspectWidget(imageLabel, 3.f/4.f);
    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(imageLabel, 1);
    this->setLayout(hLayout);

    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    QLabel *xy_label = new QLabel(); // XY label
    xy_label->setText("XY value:");
    xy_label->setFont(font);
    QLabel *xz_label = new QLabel(); // XZ label
    xz_label->setText("XZ value:");
    xz_label->setFont(font);
    QLabel *xw_label = new QLabel(); // XW label
    xw_label->setText("XW value:");
    xw_label->setFont(font);
    QLabel *yz_label = new QLabel(); // YZ label
    yz_label->setText("YZ value:");
    yz_label->setFont(font);
    QLabel *yw_label = new QLabel(); // YW label
    yw_label->setText("YW value:");
    yw_label->setFont(font);
    QLabel *zw_label = new QLabel(); // ZW label
    zw_label->setText("ZW value:");
    zw_label->setFont(font);
    QLabel *w_label = new QLabel(); // W label
    w_label->setText("W value:");
    w_label->setFont(font);

    currTimeLabel = new QLabel(); // Current Time label
    currTimeLabel->setText("Curr Time: 0");
    currTimeLabel->setFont(font);

    QLabel *max_time_label = new QLabel(); // Max Time label
    max_time_label->setText("Max Time:");
    max_time_label->setFont(font);
    

    // Create file uploader for scene file
    uploadFile = new QPushButton();
    uploadFile->setText(QStringLiteral("Upload Scene File"));
    
    saveImage = new QPushButton();
    saveImage->setText(QStringLiteral("Save image"));

    // Select directory for bulk output
    bulkRender = new QPushButton();
    bulkRender->setText(QStringLiteral("Bulk Render"));

    QGroupBox *xyLayout = new QGroupBox(); // horizontal w slider alignment
    QHBoxLayout *lxy = new QHBoxLayout();

    xySlider = new QSlider(Qt::Orientation::Horizontal); // XY value slider
    xySlider->setTickInterval(1);
    xySlider->setMinimum(0);
    xySlider->setMaximum(36000);
    xySlider->setValue(0);

    xyBox = new QDoubleSpinBox();
    xyBox->setMinimum(0.0f);
    xyBox->setMaximum(360.f);
    xyBox->setSingleStep(1.f);
    xyBox->setValue(0.f);

    lxy->addWidget(xySlider);
    lxy->addWidget(xyBox);
    xyLayout->setLayout(lxy);

    // XZ Slider
    QGroupBox *xzLayout = new QGroupBox(); // horizontal w slider alignment
    QHBoxLayout *lxz = new QHBoxLayout();

    xzSlider = new QSlider(Qt::Orientation::Horizontal); // XY value slider
    xzSlider->setTickInterval(1);
    xzSlider->setMinimum(0);
    xzSlider->setMaximum(36000);
    xzSlider->setValue(0);

    xzBox = new QDoubleSpinBox();
    xzBox->setMinimum(0.0f);
    xzBox->setMaximum(360.f);
    xzBox->setSingleStep(1.f);
    xzBox->setValue(0.f);

    lxz->addWidget(xzSlider);
    lxz->addWidget(xzBox);
    xzLayout->setLayout(lxz);

    // XW Slider
    QGroupBox *xwLayout = new QGroupBox(); // horizontal w slider alignment
    QHBoxLayout *lxw = new QHBoxLayout();

    xwSlider = new QSlider(Qt::Orientation::Horizontal); // XY value slider
    xwSlider->setTickInterval(1);
    xwSlider->setMinimum(0);
    xwSlider->setMaximum(36000);
    xwSlider->setValue(0);

    xwBox = new QDoubleSpinBox();
    xwBox->setMinimum(0.0f);
    xwBox->setMaximum(360.f);
    xwBox->setSingleStep(1.f);
    xwBox->setValue(0.f);

    lxw->addWidget(xwSlider);
    lxw->addWidget(xwBox);
    xwLayout->setLayout(lxw);

    // YZ Slider
    QGroupBox *yzLayout = new QGroupBox(); // horizontal w slider alignment
    QHBoxLayout *lyz = new QHBoxLayout();

    yzSlider = new QSlider(Qt::Orientation::Horizontal); // XY value slider
    yzSlider->setTickInterval(1);
    yzSlider->setMinimum(0);
    yzSlider->setMaximum(36000);
    yzSlider->setValue(0);

    yzBox = new QDoubleSpinBox();
    yzBox->setMinimum(0.0f);
    yzBox->setMaximum(360.f);
    yzBox->setSingleStep(1.f);
    yzBox->setValue(0.f);

    lyz->addWidget(yzSlider);
    lyz->addWidget(yzBox);
    yzLayout->setLayout(lyz);

    // YW Slider
    QGroupBox *ywLayout = new QGroupBox(); // horizontal w slider alignment
    QHBoxLayout *lyw = new QHBoxLayout();
    
    ywSlider = new QSlider(Qt::Orientation::Horizontal); // XY value slider
    ywSlider->setTickInterval(1);
    ywSlider->setMinimum(0);
    ywSlider->setMaximum(36000);
    ywSlider->setValue(0);

    ywBox = new QDoubleSpinBox();
    ywBox->setMinimum(0.0f);
    ywBox->setMaximum(360.f);
    ywBox->setSingleStep(1.f);
    ywBox->setValue(0.f);

    lyw->addWidget(ywSlider);
    lyw->addWidget(ywBox);
    ywLayout->setLayout(lyw);

    // ZW Slider
    QGroupBox *zwLayout = new QGroupBox(); // horizontal w slider alignment
    QHBoxLayout *lzw = new QHBoxLayout();

    zwSlider = new QSlider(Qt::Orientation::Horizontal); // XY value slider
    zwSlider->setTickInterval(1);
    zwSlider->setMinimum(0);
    zwSlider->setMaximum(36000);
    zwSlider->setValue(0);

    zwBox = new QDoubleSpinBox();
    zwBox->setMinimum(0.0f);
    zwBox->setMaximum(360.f);
    zwBox->setSingleStep(1.f);
    zwBox->setValue(0.f);

    lzw->addWidget(zwSlider);
    lzw->addWidget(zwBox);
    zwLayout->setLayout(lzw);

    // Max Time Slider
    QGroupBox *maxTimeGroupBox = new QGroupBox(); // horizontal 
    QHBoxLayout *maxTimeLayout = new QHBoxLayout();

    maxTimeSlider = new QSlider(Qt::Orientation::Horizontal);
    maxTimeSlider->setTickInterval(1);
    maxTimeSlider->setMinimum(0);
    maxTimeSlider->setMaximum(18000);
    maxTimeSlider->setValue(1);

    maxTimeSpinBox = new QSpinBox();
    maxTimeSpinBox->setMinimum(0);
    maxTimeSpinBox->setMaximum(600);
    maxTimeSpinBox->setSingleStep(1);
    maxTimeSpinBox->setValue(settings.rotation);

    maxTimeLayout->addWidget(maxTimeSlider);
    maxTimeLayout->addWidget(maxTimeSpinBox);
    maxTimeGroupBox->setLayout(maxTimeLayout);

    // checkbox
    // bulkRender = new QPushButton();
    // bulkRender->setText(QStringLiteral("Bulk Render"));
    rotateNegative = new QPushButton();
    rotateNegative->setText(QStringLiteral("Render the Scene!"));
    rotateNegative->setChecked(false);

    // w Slider
    QGroupBox *wLayout = new QGroupBox(); // horizontal w slider alignment
    QHBoxLayout *lw = new QHBoxLayout();

    wSlider = new QSlider(Qt::Orientation::Horizontal); // XY value slider
    wSlider->setTickInterval(1);
    wSlider->setMinimum(-10000);
    wSlider->setMaximum(10000);
    wSlider->setValue(0);

    wBox = new QDoubleSpinBox();
    wBox->setMinimum(-100.0f);
    wBox->setMaximum(100.f);
    wBox->setSingleStep(0.1f);
    wBox->setValue(0.f);

    lw->addWidget(wSlider);
    lw->addWidget(wBox);
    wLayout->setLayout(lw);


    vLayout->addWidget(uploadFile);
    vLayout->addWidget(saveImage);
    vLayout->addWidget(xy_label);
    vLayout->addWidget(xyLayout);
    vLayout->addWidget(xz_label);
    vLayout->addWidget(xzLayout);
    vLayout->addWidget(xw_label);
    vLayout->addWidget(xwLayout);
    vLayout->addWidget(yz_label);
    vLayout->addWidget(yzLayout);
    vLayout->addWidget(yw_label);
    vLayout->addWidget(ywLayout);
    vLayout->addWidget(zw_label);
    vLayout->addWidget(zwLayout);
    vLayout->addWidget(w_label);
    vLayout->addWidget(wLayout);
    vLayout->addWidget(currTimeLabel);
    vLayout->addWidget(max_time_label);

    vLayout->addWidget(maxTimeGroupBox);
    vLayout->addWidget(bulkRender);
    vLayout->addWidget(rotateNegative);

    connectUIElements();

    onValChangeWSlider(0);
}

void MainWindow::finish() {
//    realtime->finish();
//    delete(rayTracer);
}

void MainWindow::connectUIElements() {
    connectUploadFile();
    connectSaveImage();
    connectBulkRender();
    connectxy();
    connectxz();
    connectxw();
    connectyz();
    connectyw();
    connectzw();
    connectMaxTimeSlider();
    connectNegativeRotation();
    connect(rayTracer, &RayTracer::xyRotationChanged, this, &MainWindow::updateXySlider);
    connect(rayTracer, &RayTracer::xzRotationChanged, this, &MainWindow::updateXzSlider);
    connect(rayTracer, &RayTracer::xwRotationChanged, this, &MainWindow::updateXwSlider);
    connect(rayTracer, &RayTracer::yzRotationChanged, this, &MainWindow::updateYzSlider);
    connect(rayTracer, &RayTracer::ywRotationChanged, this, &MainWindow::updateYwSlider);
    connect(rayTracer, &RayTracer::zwRotationChanged, this, &MainWindow::updateZwSlider);
    connect(rayTracer, &RayTracer::rotationChanged, this, &MainWindow::updateRotationSlider);
    // connect(rayTracer, &RayTracer::cameraPositionChanged, this, &MainWindow::updateCameraPosition);
    connect(rayTracer, &RayTracer::timeValueChanged, this, &MainWindow::updateTimeValue);
    connectW();
}

void MainWindow::connectUploadFile() {
    connect(uploadFile, &QPushButton::clicked, this, &MainWindow::onUploadFile);
}

void MainWindow::connectSaveImage() {
    connect(saveImage, &QPushButton::clicked, this, &MainWindow::onSaveImage);
}

void MainWindow::connectBulkRender() {
    connect(bulkRender, &QPushButton::clicked, this, &MainWindow::onBulkRender);
}

void MainWindow::connectxy() {
    connect(xySlider, &QSlider::valueChanged, this, &MainWindow::onValChangexySlider);
    connect(xyBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangexyBox);
}

void MainWindow::connectxz() {
    connect(xzSlider, &QSlider::valueChanged, this, &MainWindow::onValChangexzSlider);
    connect(xzBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangexzBox);
}

void MainWindow::connectxw() {
    connect(xwSlider, &QSlider::valueChanged, this, &MainWindow::onValChangexwSlider);
    connect(xwBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangexwBox);
}

void MainWindow::connectyz() {
    connect(yzSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeyzSlider);
    connect(yzBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeyzBox);
}

void MainWindow::connectyw() {
    connect(ywSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeywSlider);
    connect(ywBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeywBox);
}

void MainWindow::connectzw() {
    connect(zwSlider, &QSlider::valueChanged, this, &MainWindow::onValChangezwSlider);
    connect(zwBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangezwBox);
}

void MainWindow::connectMaxTimeSlider() {
    connect(maxTimeSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeMaxTimeSlider);
    connect(maxTimeSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &MainWindow::onValChangeMaxTimeBox);
}

void MainWindow::connectNegativeRotation() {
    connect(rotateNegative, &QPushButton::clicked, this, &MainWindow::onRotateNegative);
}

void MainWindow::connectW() {
    connect(wSlider, &QSlider::valueChanged, this, &MainWindow::onValChangeWSlider);
    connect(wBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onValChangeWBox);
}

void MainWindow::onUploadFile() {
    // Get abs path of scene file
    QString configFilePath = QFileDialog::getOpenFileName(this, tr("Upload File"),
                                                          QDir::currentPath()
                                                              .append(QDir::separator())
                                                              .append("scenefiles")
                                                              .append(QDir::separator())
                                                              .append("lights-camera")
                                                              .append(QDir::separator())
                                                              .append("required"), tr("Scene Files (*.json)"));
    if (configFilePath.isNull()) {
        std::cout << "Failed to load null scenefile." << std::endl;
        return;
    }

    settings.sceneFilePath = configFilePath.toStdString();

    std::cout << "Loaded scenefile: \"" << configFilePath.toStdString() << "\"." << std::endl;

    rayTracer->sceneChanged(imageLabel);
}

void MainWindow::onSaveImage() {
    // if (settings.sceneFilePath.empty()) {
    //     std::cout << "No scene file loaded." << std::endl;
    //     return;
    // }
    std::string sceneName = settings.sceneFilePath.substr(0, settings.sceneFilePath.find_last_of("."));
    sceneName = sceneName.substr(sceneName.find_last_of("/")+1);
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    QDir::currentPath()
                                                        .append(QDir::separator())
                                                        .append(sceneName), tr("Image Files (*.png)"));
    std::cout << "Saving image to: \"" << filePath.toStdString() << "\"." << std::endl;
    rayTracer->saveViewportImage(filePath.toStdString());
}

void MainWindow::onBulkRender() {
    // if (settings.sceneFilePath.empty()) {
    //     std::cout << "No scene file loaded." << std::endl;
    //     return;
    // }
    std::string sceneName = settings.sceneFilePath.substr(0, settings.sceneFilePath.find_last_of("."));
    sceneName = sceneName.substr(sceneName.find_last_of("/")+1);
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Directory for Bulk Render"),
                                                           QDir::currentPath());
    std::cout << "Setting bulk output path to: \"" << folderPath.toStdString() << "\"." << std::endl;
    settings.bulkOutputFolderPath = folderPath.toStdString();
    rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangexySlider(int newValue) {
    //wSlider->setValue(newValue);
    xyBox->setValue(newValue/100.f);
    settings.xy = xyBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangexyBox(double newValue) {
    xySlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.xy = xyBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangexzSlider(int newValue) {
    //wSlider->setValue(newValue);
    xzBox->setValue(newValue/100.f);
    settings.xz = xzBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangexzBox(double newValue) {
    xzSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.xz = xzBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangexwSlider(int newValue) {
    //wSlider->setValue(newValue);
    xwBox->setValue(newValue/100.f);
    settings.xw = xwBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangexwBox(double newValue) {
    xwSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.xw = xwBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeyzSlider(int newValue) {
    //wSlider->setValue(newValue);
    yzBox->setValue(newValue/100.f);
    settings.yz = yzBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeyzBox(double newValue) {
    yzSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.yz = yzBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeywSlider(int newValue) {
    //wSlider->setValue(newValue);
    ywBox->setValue(newValue/100.f);
    settings.yw = ywBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeywBox(double newValue) {
    ywSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.yw = ywBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangezwSlider(int newValue) {
    //wSlider->setValue(newValue);
    zwBox->setValue(newValue/100.f);
    settings.zw = zwBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangezwBox(double newValue) {
    zwSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.zw = zwBox->value();
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeMaxTimeSlider(int newValue) {
    //wSlider->setValue(newValue);
    maxTimeSpinBox->setValue(newValue/100.f);
    settings.maxTime = maxTimeSpinBox->value();
    rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeMaxTimeBox(double newValue) {
    maxTimeSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.rotation = maxTimeSpinBox->value();
    rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeWSlider(int newValue) {
    //wSlider->setValue(newValue);
    wBox->setValue(newValue/100.f);
    settings.w = wBox->value();
    rayTracer->settingsChanged(imageLabel);
    // ray->wSliderChanged(imageLabel);
}

void MainWindow::onValChangeWBox(double newValue) {
    wSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.w = wBox->value();
    rayTracer->settingsChanged(imageLabel);
    // ray->wSliderChanged(imageLabel);
}

void MainWindow::onRotateNegative() {
    // settings.negative = !settings.negative;
    rayTracer->settingsChanged(imageLabel);
}

void MainWindow::updateXySlider(double value) {
    xySlider->setValue(int(value*100.f));
    xyBox->setValue(value);
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::updateXzSlider(double value) {
    xzSlider->setValue(int(value*100.f));
    xzBox->setValue(value);
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::updateXwSlider(double value) {
    xwSlider->setValue(int(value*100.f));
    xwBox->setValue(value);
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::updateYzSlider(double value) {
    yzSlider->setValue(int(value*100.f));
    yzBox->setValue(value);
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::updateYwSlider(double value) {
    ywSlider->setValue(int(value*100.f));
    ywBox->setValue(value);
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::updateZwSlider(double value) {
    zwSlider->setValue(int(value*100.f));
    zwBox->setValue(value);
    // rayTracer->settingsChanged(imageLabel);
}

void MainWindow::updateRotationSlider(float value) {
    maxTimeSlider->setValue(int(value*100.f));
    maxTimeSpinBox->setValue(value);
    rayTracer->settingsChanged(imageLabel);
}

// void MainWindow::updateCameraPosition() {
//     rayTracer->wSliderChanged(imageLabel);
// }

void MainWindow::updateTimeValue() {
    currTimeLabel->setText("Curr Time: " + QString::number(settings.currentTime));
}
