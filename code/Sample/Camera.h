#ifndef CAMERA_H
#define CAMERA_H

#include <QtMath>
#include <QTime>
#include <QTimer>
#include <QElapsedTimer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <vector>

#include <QOpenGLFunctions_3_3_Core>

#include <cstdio>
#include <cstring>


// 移动方向枚举量. 是一种抽象，以避开特定于窗口系统的输入方法
// 我们这里是WSAD
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 默认值
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// 一个抽象的camera类，用于处理输入并计算相应的Euler角度、向量和矩阵，以便在OpenGL中使用
class Camera
{
public:
    // camera Attributes
    QVector3D Position;
    QVector3D Front;
    QVector3D Up;
    QVector3D Right;
    QVector3D WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    static const float PITCH_UPPER_BOUND;
    static const float PITCH_LOWER_BOUND;


    float lastMoveTime;
    QElapsedTimer timer;

    std::map<int, bool> keys;
    bool drag;
    bool firstMouse;
    float lastX, lastY;
    float curFrame, lastFrame, deltaTime;

    // constructor with vectors
    Camera(QVector3D position = QVector3D(0.0f, 0.0f, 0.0f), QVector3D up = QVector3D(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    ~Camera();

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    QMatrix4x4 GetViewMatrix();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void processMouseMovement(float xoffset, float yoffset);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    // 处理从任何类似键盘的输入系统接收的输入。接受摄像机定义枚举形式的输入参数（从窗口系统中提取）
    void processKeyboard();
    // 处理从鼠标滚轮事件接收的输入。仅需要在垂直车轮轴上输入

    void processMouseScroll(float yoffset);

    void lookTo(QVector3D direction);
    void lookTo(float _pitch, float _yaw);
private:
    // 根据相机的（更新的）Euler角度计算前矢量
    void updateCameraVectors();

};
#endif

