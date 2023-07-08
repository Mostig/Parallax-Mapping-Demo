#ifndef AXBOPENGLWIDGET_H
#define AXBOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include <QFile>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>

#include "Camera.h"
#include "model.h"

class AXBOpenGLWidget: public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    QVector3D lightPos;

    GLuint cubeVAO, VBO, lightCubeVAO, quadVAO = 0, quadVBO;
    GLuint shaderProgram, phongShaderProgram, lightCubeShaderProgram, normalMappingShader,parallaxMappingShader,usingShaderProgram;
    GLuint diffuseMap, normalMap, heightMap;
    QMatrix4x4 model, view, projection;

    // 使用 c++ STL 中的动态数组 vector 存储所有顶点属性（位置）的数据
    //std::vector<QVector3D> vertices;
    // 法向
    //std::vector<QVector3D> normals;
    // 使用 c++ STL 中的动态数组 vector 存储所有三角片的顶点下标
    //std::vector<unsigned int> indices;

    Camera * m_camera;
    Model * ourModel;

public:
    //enum MODEL_TYPE{MESH, POINT_CLOUD, VOLUME, NURBS};
    //enum LOAD_FLAG {LOADMESH, NONE};
    enum MODE{MODEL_LOADER, BUMP_DEMO};
    explicit AXBOpenGLWidget(QWidget *parent = nullptr);
    ~AXBOpenGLWidget();

    void loadModel(QString filePath);
    void setWireframe(bool wireframe);
    void enablePhongLightingMode();
    void enableBlinnPhongLightingMode();
    void showTexture();
    void setNormalMap(bool normal);
    void loadBumpMappingDemo();
    void setNormalMappingMode();
    void setParallaxMappingMode(int mode);
    void setRotateAngle(int newRotateAngle);

    int getCurrentframes() const;

protected:
    virtual void initializeGL();

    virtual void resizeGL(int w, int h);
    virtual void paintGL();

    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

private slots:


private:
    int currentframes = 0, frames = 0; // 帧率
    MODE my_mode = MODEL_LOADER;
    QTimer timer;
    QElapsedTimer m_time;
    QPoint deltaPos;
    int rotateAngle;
    void showEvent(QShowEvent *event); //重载QOpenGLWidget的showEvent函数，设置焦点到子窗口
    GLuint createLinkShader(QString vertexShaderFielName, QString fragShaderFileName); //创建、编译并链接shader
    GLuint loadTexture(const char* path);
    void RenderQuad();
    void calFramesPerSecond();
signals:

};

#endif // AXBOPENGLWIDGET_H
