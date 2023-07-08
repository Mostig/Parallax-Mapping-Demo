#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setCentralWidget(ui->openGLWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionMesh_triggered()
{
    QString OpenFileName, OpenFilePath;

    OpenFileName = QFileDialog::getOpenFileName(this,
    "please choose an Mesh Model file",
    "",
    "Mesh Model Files(*.obj *.fbx *.stl);;All(*.*)");

    if (!OpenFileName.isNull())
    {
        ui->openGLWidget->loadModel(OpenFileName);
        /*
        my_widget = new AXBOpenGLWidget(OpenFileName, this);
        setCentralWidget(my_widget);
        //my_widget->setFocus();
        connect(ui->actionPolygonMode, SIGNAL(triggered()), my_widget, SLOT(enablePolygonMode()));

        connect(ui->actionPhong, SIGNAL(triggered()), my_widget, SLOT(enablePhongLightingMode()));
        connect(ui->actionBlinn_Phong, SIGNAL(triggered()), my_widget, SLOT(enableBlinnPhongLightingMode()));

        connect(ui->actionShowTexture, SIGNAL(triggered()), my_widget, SLOT(showTexture()));

        //ui->openGLWidget->objLoader(OpenFileName);
        */
    }
}


void MainWindow::on_actionPolygonMode_triggered()
{
    ui->openGLWidget->setWireframe(true);
}


void MainWindow::on_actionPhong_triggered()
{
    ui->openGLWidget->setWireframe(false);
    ui->openGLWidget->enablePhongLightingMode();
}


void MainWindow::on_actionBlinn_Phong_triggered()
{
    ui->openGLWidget->setWireframe(false);
    ui->openGLWidget->enableBlinnPhongLightingMode();
}

void MainWindow::on_actionShowTexture_triggered()
{
    ui->openGLWidget->showTexture();
}


/*
void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1) // checked = 2
    {
        ui->openGLWidget->setNormalMap(true);
    }
    else // unchecked = 0
    {
        ui->openGLWidget->setNormalMap(false);
    }
}
*/

void MainWindow::on_bumpMappingButton_clicked()
{
    ui->openGLWidget->loadBumpMappingDemo();
}

void MainWindow::on_rotateSlider_valueChanged(int value)
{
    ui->showAngleLabel->setText(QString::number(value));
    ui->openGLWidget->setRotateAngle(value);
}

void MainWindow::on_normalMappingButton_toggled(bool checked)
{
    if(checked)
    {
        ui->openGLWidget->setNormalMappingMode();
    }
}

void MainWindow::on_parallaxMappingButton_toggled(bool checked)
{
    if(checked)
    {
        ui->openGLWidget->setParallaxMappingMode(0);
    }
}

void MainWindow::on_deepParallaxMappingButton_toggled(bool checked)
{
    if(checked)
    {
        ui->openGLWidget->setParallaxMappingMode(1);
    }
}

void MainWindow::on_parallaxOcclusionMappingButton_toggled(bool checked)
{
    if(checked)
    {
        ui->openGLWidget->setParallaxMappingMode(2);
    }
}




