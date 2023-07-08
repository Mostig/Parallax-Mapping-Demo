#include "Camera.h"

const float Camera::PITCH_UPPER_BOUND = 89.0f;
const float Camera::PITCH_LOWER_BOUND = -89.0f;
// constructor with vectors
Camera::Camera(QVector3D position, QVector3D up, float yaw, float pitch):
    Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    lastFrame = lastMoveTime = -1.0f;
    drag = false;
    firstMouse = true;

    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();

    timer.start();
}
// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    lastFrame = lastMoveTime = -1.0f;
    drag = false;
    firstMouse = true;

    Position = QVector3D(posX, posY, posZ);
    WorldUp = QVector3D(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();

    timer.start();
}

Camera::~Camera()
{
}

QMatrix4x4 Camera::GetViewMatrix()
{
    QMatrix4x4 theMatrix;
    theMatrix.lookAt(Position, Position + Front, Up);
    return theMatrix;
}


void Camera::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        drag = true;
        firstMouse = true;
    }
}

void Camera::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        drag = false;
    }
}

// 处理从鼠标输入系统接收的输入。需要x和y方向上的偏移值。
void Camera::mouseMoveEvent(QMouseEvent *event)
{
    if(drag){
        double xpos = event->x();
        double ypos = event->y();

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
            return;
        }

        float sensitivity = 0.1f;
        float xoffset = (xpos - lastX) * sensitivity;
        float yoffset = (lastY - ypos) * sensitivity; //

        processMouseMovement(xoffset, yoffset);

        lastX = xpos;
        lastY = ypos;
    }

    // 使用更新的Euler角度更新前、右和上矢量
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 75.0f)
        Zoom = 75.0f;
}


// 视线指向 direction
void Camera::lookTo(QVector3D direction)
{
    float eps = 1e-6;
    direction.normalize();
    // direction 不能与 worldUp 同向或反向
    if (1.0 - qFabs(QVector3D::dotProduct(direction, WorldUp)) < eps) {
        //qDebug() << qFabs(QVector3D::dotProduct(direction, worldUp)) << endl;
        return;
    }

    Pitch = qAsin(direction.y());
    float cosPitch = qSqrt(1 - direction.y() * direction.y());
    float cosYaw = direction.x() / cosPitch;
    float sinYaw = direction.z() / cosPitch;
    float t = qSqrt(cosYaw * cosYaw + sinYaw * sinYaw);
    cosYaw = cosYaw / t;
    sinYaw = sinYaw / t;
    if (cosYaw >= -eps) Yaw = qAsin(sinYaw);
    else if (sinYaw >= -eps) Yaw = qAcos(cosYaw);
    else Yaw = qAtan(sinYaw / cosYaw) + M_PI;

    lookTo(qRadiansToDegrees(Pitch), qRadiansToDegrees(Yaw));

}

void Camera::lookTo(float _pitch, float _yaw)
{
    if (_pitch > PITCH_UPPER_BOUND || _pitch < PITCH_LOWER_BOUND) return;
    Pitch = _pitch;
    Yaw = _yaw;
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    float PI=3.1415926;
    QVector3D front;
    front.setX(cos(Yaw*PI/180.0) * cos(Pitch*PI/180.0));
    front.setY(sin(Pitch*PI/180.0));
    front.setZ(sin(Yaw*PI/180.0) * cos(Pitch*PI/180.0));
    front.normalize();
    Front = front;
    // also re-calculate the Right and Up vector
    Right = QVector3D::crossProduct(Front, WorldUp);
    // 标准化向量，因为向上或向下看得越多，向量的长度就越接近0，这会导致移动速度变慢。
    Right.normalize();
    Up    = QVector3D::crossProduct(Right, Front);
    Up.normalize();
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > PITCH_UPPER_BOUND) Pitch = PITCH_UPPER_BOUND;
    if (Pitch < PITCH_LOWER_BOUND) Pitch = PITCH_LOWER_BOUND;

    updateCameraVectors();
}

void Camera::wheelEvent(QWheelEvent *event)
{
    processMouseScroll(event->angleDelta().y()/120);
}

void Camera::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    keys[event->key()] = true;
    if (lastMoveTime < 0.0f) {
        if (
            key == Qt::Key_W ||
            key == Qt::Key_S ||
            key == Qt::Key_A ||
            key == Qt::Key_D ||
            key == Qt::Key_Space ||
            key == Qt::Key_Shift
            ) {
            lastMoveTime = timer.elapsed() * 1.0 / 1000;
        }
    }
}

void Camera::keyReleaseEvent(QKeyEvent *event)
{
    keys[event->key()] = false;
}

void Camera::processKeyboard()
{
    curFrame = timer.elapsed() * 1.0 / 1000;
    if (lastFrame >= 0.0f)
        deltaTime = curFrame - lastFrame;
    else
        deltaTime = 0.0f;
    lastFrame = curFrame;

    if (
        (keys.find(Qt::Key_W) == keys.end() || keys[Qt::Key_W] == false) &&
        (keys.find(Qt::Key_S) == keys.end() || keys[Qt::Key_S] == false) &&
        (keys.find(Qt::Key_A) == keys.end() || keys[Qt::Key_A] == false) &&
        (keys.find(Qt::Key_D) == keys.end() || keys[Qt::Key_D] == false) &&
        (keys.find(Qt::Key_Space) == keys.end() || keys[Qt::Key_Space] == false) &&
        (keys.find(Qt::Key_Shift) == keys.end() || keys[Qt::Key_Shift] == false)
        ) {
        lastMoveTime = -1.0f;
    }

    float velocity = MovementSpeed * deltaTime;
    if (lastMoveTime > 0.0f)
        velocity *= (1.0 + curFrame - lastMoveTime);
    if (keys[Qt::Key_W]) Position += Front * velocity;
    if (keys[Qt::Key_S]) Position -= Front * velocity;
    if (keys[Qt::Key_A]) Position -= Right * velocity;
    if (keys[Qt::Key_D]) Position += Right * velocity;
    if (keys[Qt::Key_Space]) Position += WorldUp * velocity;
    if (keys[Qt::Key_Shift]) Position -= WorldUp * velocity;

}
