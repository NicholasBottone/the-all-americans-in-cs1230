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
    QLabel *w_label = new QLabel(); // Width label
    w_label->setText("W value:");

    // Create file uploader for scene file
    uploadFile = new QPushButton();
    uploadFile->setText(QStringLiteral("Upload Scene File"));
    
    saveImage = new QPushButton();
    saveImage->setText(QStringLiteral("Save image"));

    QGroupBox *wLayout = new QGroupBox(); // horizonal w slider alignment
    QHBoxLayout *lw = new QHBoxLayout();

    wSlider = new QSlider(Qt::Orientation::Horizontal); // W value slider
    wSlider->setTickInterval(1);
    wSlider->setMinimum(1);
    wSlider->setMaximum(100);
    wSlider->setValue(1);

    wBox = new QDoubleSpinBox();
    wBox->setMinimum(0.01f);
    wBox->setMaximum(1.f);
    wBox->setSingleStep(0.01f);
    wBox->setValue(0.01f);

    lw->addWidget(wSlider);
    lw->addWidget(wBox);
    wLayout->setLayout(lw);

    vLayout->addWidget(uploadFile);
    vLayout->addWidget(saveImage);
    vLayout->addWidget(w_label);
    vLayout->addWidget(wLayout);

    connectUIElements();

    onValChangeWBox(0.01f);
}

void MainWindow::finish() {
//    realtime->finish();
//    delete(realtime);
}

void MainWindow::connectUIElements() {
    connectUploadFile();
    connectSaveImage();
    connectW();
}

void MainWindow::connectUploadFile() {
    connect(uploadFile, &QPushButton::clicked, this, &MainWindow::onUploadFile);
}

void MainWindow::connectSaveImage() {
    connect(saveImage, &QPushButton::clicked, this, &MainWindow::onSaveImage);
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
    if (settings.sceneFilePath.empty()) {
        std::cout << "No scene file loaded." << std::endl;
        return;
    }
    std::string sceneName = settings.sceneFilePath.substr(0, settings.sceneFilePath.find_last_of("."));
    sceneName = sceneName.substr(sceneName.find_last_of("/")+1);
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"),
                                                    QDir::currentPath()
                                                        .append(QDir::separator())
                                                        .append("student_outputs")
                                                        .append(QDir::separator())
                                                        .append("lights-camera")
                                                        .append(QDir::separator())
                                                        .append("required")
                                                        .append(QDir::separator())
                                                        .append(sceneName), tr("Image Files (*.png)"));
    std::cout << "Saving image to: \"" << filePath.toStdString() << "\"." << std::endl;
//    realtime->saveViewportImage(filePath.toStdString());
}

void MainWindow::onValChangeWSlider(int newValue) {
    //wSlider->setValue(newValue);
    wBox->setValue(newValue/100.f);
    settings.w = wBox->value();
    rayTracer->settingsChanged(imageLabel);
}

void MainWindow::onValChangeWBox(double newValue) {
    wSlider->setValue(int(newValue*100.f));
    //wBox->setValue(newValue);
    settings.w = wBox->value();
    rayTracer->settingsChanged(imageLabel);
}
