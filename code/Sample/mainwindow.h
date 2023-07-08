#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "axbopenglwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionMesh_triggered();

    void on_actionPolygonMode_triggered();

    void on_actionPhong_triggered();

    void on_actionBlinn_Phong_triggered();

    void on_actionShowTexture_triggered();

    void on_bumpMappingButton_clicked();

    void on_normalMappingButton_toggled(bool checked);

    void on_parallaxMappingButton_toggled(bool checked);

    void on_rotateSlider_valueChanged(int value);

    void on_deepParallaxMappingButton_toggled(bool checked);

    void on_parallaxOcclusionMappingButton_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    AXBOpenGLWidget *my_widget = nullptr;
};
#endif // MAINWINDOW_H
