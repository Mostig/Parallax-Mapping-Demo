#include "axbopenglwidget.h"
#define TIMEOUTMSEC 100
#include <iostream>
#include <sstream>
using namespace std;

AXBOpenGLWidget::AXBOpenGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}


AXBOpenGLWidget::~AXBOpenGLWidget()
{
    makeCurrent();
    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1,&EBO);

    glDeleteProgram(shaderProgram);
    doneCurrent();
}

void AXBOpenGLWidget::loadModel(QString filePath)
{
    my_mode = MODEL_LOADER;
    usingShaderProgram = shaderProgram;
    makeCurrent();
    ourModel = new Model(filePath.toStdString());
    update();
    doneCurrent();
}

void AXBOpenGLWidget::setWireframe(bool wireframe)
{
    makeCurrent();
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    update();
    doneCurrent();
}

void AXBOpenGLWidget::enablePhongLightingMode()
{
    makeCurrent();
    //usingShaderProgram = phongShaderProgram;
    glUseProgram(usingShaderProgram);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "blinn"), false);
    update();
    doneCurrent();
}

void AXBOpenGLWidget::enableBlinnPhongLightingMode()
{
    makeCurrent();
    //usingShaderProgram = phongShaderProgram;
    glUseProgram(usingShaderProgram);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "blinn"), true);
    update();
    doneCurrent();
}

void AXBOpenGLWidget::showTexture()
{
    makeCurrent();
    usingShaderProgram = shaderProgram;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    update();
    doneCurrent();
}

void AXBOpenGLWidget::setNormalMap(bool normal)
{
    makeCurrent();
    glUseProgram(usingShaderProgram);
    if(normal)
        glUniform1i(glGetUniformLocation(usingShaderProgram, "normalMap"), true);
    else
        glUniform1i(glGetUniformLocation(usingShaderProgram, "normalMap"), false);
    update();
    doneCurrent();
}

void AXBOpenGLWidget::loadBumpMappingDemo()
{
    // 加载凹凸映射前的初始化操作
    if(ourModel != NULL)
    {
        delete ourModel;
        ourModel = NULL;
    }
    my_mode = BUMP_DEMO;
    usingShaderProgram = phongShaderProgram;

    makeCurrent();
    // positions
    QVector3D pos1(-1.0, 1.0, 0.0);
    QVector3D pos2(-1.0, -1.0, 0.0);
    QVector3D pos3(1.0, -1.0, 0.0);
    QVector3D pos4(1.0, 1.0, 0.0);
    // texture coordinates
    QVector2D uv1(0.0, 1.0);
    QVector2D uv2(0.0, 0.0);
    QVector2D uv3(1.0, 0.0);
    QVector2D uv4(1.0, 1.0);
    // normal vector
    QVector3D nm(0.0, 0.0, 1.0);

    // calculate tangent/bitangent vectors of both triangles
    QVector3D tangent1, bitangent1;
    QVector3D tangent2, bitangent2;
    // - triangle 1
    QVector3D edge1 = pos2 - pos1;
    QVector3D edge2 = pos3 - pos1;
    QVector2D deltaUV1 = uv2 - uv1;
    QVector2D deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    tangent1.setX(f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()));
    tangent1.setY(f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()));
    tangent1.setZ(f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()));
    tangent1.normalize();

    bitangent1.setX(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()));
    bitangent1.setY(f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()));
    bitangent1.setZ(f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));
    bitangent1.normalize();

    // - triangle 2
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

    tangent2.setX(f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()));
    tangent2.setY(f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()));
    tangent2.setZ(f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()));
    tangent2.normalize();


    bitangent2.setX(f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()));
    bitangent2.setY(f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()));
    bitangent2.setZ(f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()));
    bitangent2.normalize();


    GLfloat quadVertices[] = {
        // Positions                  // normal               // TexCoords      // Tangent                                // Bitangent
        pos1.x(), pos1.y(), pos1.z(), nm.x(), nm.y(), nm.z(), uv1.x(), uv1.y(), tangent1.x(), tangent1.y(), tangent1.z(), bitangent1.x(), bitangent1.y(), bitangent1.z(),
        pos2.x(), pos2.y(), pos2.z(), nm.x(), nm.y(), nm.z(), uv2.x(), uv2.y(), tangent1.x(), tangent1.y(), tangent1.z(), bitangent1.x(), bitangent1.y(), bitangent1.z(),
        pos3.x(), pos3.y(), pos3.z(), nm.x(), nm.y(), nm.z(), uv3.x(), uv3.y(), tangent1.x(), tangent1.y(), tangent1.z(), bitangent1.x(), bitangent1.y(), bitangent1.z(),

        pos1.x(), pos1.y(), pos1.z(), nm.x(), nm.y(), nm.z(), uv1.x(), uv1.y(), tangent2.x(), tangent2.y(), tangent2.z(), bitangent2.x(), bitangent2.y(), bitangent2.z(),
        pos3.x(), pos3.y(), pos3.z(), nm.x(), nm.y(), nm.z(), uv3.x(), uv3.y(), tangent2.x(), tangent2.y(), tangent2.z(), bitangent2.x(), bitangent2.y(), bitangent2.z(),
        pos4.x(), pos4.y(), pos4.z(), nm.x(), nm.y(), nm.z(), uv4.x(), uv4.y(), tangent2.x(), tangent2.y(), tangent2.z(), bitangent2.x(), bitangent2.y(), bitangent2.z()
    };
    // Setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));

    // Load textures
    diffuseMap = loadTexture("../resources/textures/bricks2.jpg");
    normalMap = loadTexture("../resources/textures/bricks2_normal.jpg");
    heightMap = loadTexture("../resources/textures/bricks2_disp.jpg");

    // Set texture units
    glUseProgram(usingShaderProgram);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "diffuseMap"), 0);

    update();
    doneCurrent();
}

void AXBOpenGLWidget::setNormalMappingMode()
{
    usingShaderProgram = normalMappingShader;
    makeCurrent();
    // Set texture units
    glUseProgram(usingShaderProgram);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "normalMap"), 1);
    update();
    doneCurrent();
}

void AXBOpenGLWidget::setParallaxMappingMode(int mode)
{
    usingShaderProgram = parallaxMappingShader;
    makeCurrent();
    // Set texture units
    glUseProgram(usingShaderProgram);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "diffuseMap"), 0);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "normalMap"), 1);
    glUniform1i(glGetUniformLocation(usingShaderProgram, "depthMap"), 2);
    glUniform1f(glGetUniformLocation(usingShaderProgram, "heightScale"), 0.1f);

    // set mode:0--Parallax Mapping 1--Steep Parallax Mapping 2--Parallax Occlusion Mapping
    glUniform1i(glGetUniformLocation(usingShaderProgram, "ParallaxMappingMode"), mode);

    update();
    doneCurrent();
}

void AXBOpenGLWidget::showEvent(QShowEvent *event)
{
    if(this->focusWidget()){
        qDebug() << this->focusWidget()->objectName();
    }
    this->setFocus();
    QWidget::showEvent(event);
}

GLuint AXBOpenGLWidget::createLinkShader(QString vertexShaderFielName, QString fragShaderFileName)
{
    initializeOpenGLFunctions();
    QFile vertexShaderFile, fragmentShaderFile;
    vertexShaderFile.setFileName(vertexShaderFielName);
    fragmentShaderFile.setFileName(fragShaderFileName);
    if (!vertexShaderFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Vertex shader file open failed." << Qt::endl;
        return 0;
    }
    if (!fragmentShaderFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Fragment shader file open failed." << Qt::endl;
        return 0;
    }

    QTextStream vertexShaderTextStream(&vertexShaderFile);
    QTextStream fragmentShaderTextStream(&fragmentShaderFile);
    QByteArray vertexShaderSourceTemp = vertexShaderTextStream.readAll().toLocal8Bit();
    const char * vertexShaderSource = vertexShaderSourceTemp.data();
    QByteArray fragmentShaderSourceTemp = fragmentShaderTextStream.readAll().toLocal8Bit();
    const char * fragmentShaderSource = fragmentShaderSourceTemp.data();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    GLuint vertexShader,fragmentShader;

    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << Qt::endl;
    }

    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << Qt::endl;
    }


    // link shaders
    GLuint shaderProgram_ = glCreateProgram();

    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);

    glLinkProgram(shaderProgram_);
    // check for linking errors
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << Qt::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram_;
}

void AXBOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    shaderProgram = createLinkShader(":/shader.vert", ":/shader.frag");
    lightCubeShaderProgram = createLinkShader(":/lightCubeShader.vert", ":/lightCubeShader.frag");

    phongShaderProgram = createLinkShader(":/phong.vert", ":/phong.frag"); //只使用漫反射贴图
    normalMappingShader = createLinkShader(":/normal_mapping.vert", ":/normal_mapping.frag"); //使用法线贴图
    parallaxMappingShader = createLinkShader(":/parallax_mapping.vert", ":/parallax_mapping.frag"); //使用视差贴图

    usingShaderProgram = shaderProgram;

    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    // 启用 depth test，zBuffer
    glEnable(GL_DEPTH_TEST);

    // configure the light's VAO and VBO
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(lightCubeVAO);

    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    m_camera = new Camera();
    m_camera->Position= QVector3D(0.0f, 0.0f, 3.0f);
    m_camera->lookTo(QVector3D(0.0f, 0.0f, -1.0f));
    //lightPos = QVector3D(100.0f, 100.0f, 100.0f);
    lightPos = QVector3D(0.5f, 1.0f, 0.3f);

    m_time.start();
}


void AXBOpenGLWidget::resizeGL(int w, int h)
{
    makeCurrent(); // necessary
    glViewport(0,0,w,h);
}

void AXBOpenGLWidget::paintGL()
{
    update();
    m_camera->processKeyboard();

    glClearColor(0.2f,0.3f,0.3f,1.0f);
    // 清除 color buffer 和 depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = QMatrix4x4();
    view = QMatrix4x4();
    projection = QMatrix4x4();
    view.lookAt(m_camera->Position, m_camera->Position + m_camera->Front, m_camera->WorldUp);
    projection.perspective(m_camera->Zoom, (float)width()/height(), 0.1, 10000.0f);

    glUseProgram(usingShaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(usingShaderProgram, "view"), 1, GL_FALSE, view.constData());
    glUniformMatrix4fv(glGetUniformLocation(usingShaderProgram, "proj"), 1, GL_FALSE, projection.constData());

    glUniform3f(glGetUniformLocation(usingShaderProgram, "viewPos"), m_camera->Position.x(), m_camera->Position.y(), m_camera->Position.z());
    glUniform3f(glGetUniformLocation(usingShaderProgram, "lightPos"), lightPos.x(), lightPos.y(), lightPos.z());

    if(my_mode == MODEL_LOADER) //绘制导入的模型
    {
        // 将变量传递到 shader （从 CPU 传递到 GPU）（变化后需要重新传）
        glUniformMatrix4fv(glGetUniformLocation(usingShaderProgram, "model"), 1, GL_FALSE, model.constData());

        //objectColor
        glUniform3f(glGetUniformLocation(usingShaderProgram, "objectColor"), 0.5f, 0.2f, 0.2f);

        //light
        glUniform3f(glGetUniformLocation(usingShaderProgram, "light.ambient"), 0.9f, 0.9f, 0.9f);
        glUniform3f(glGetUniformLocation(usingShaderProgram, "light.diffuse"), 0.5f, 0.5f, 0.5f); // 将光照调暗了一些以搭配场景
        glUniform3f(glGetUniformLocation(usingShaderProgram, "light.specular"), 1.0f, 1.0f, 1.0f);

        if(ourModel != NULL)
        {
            ourModel->Draw(usingShaderProgram);
        }
    }
    else if(my_mode == BUMP_DEMO)
    {
        // Render quad
        model= QMatrix4x4();
        model.rotate(rotateAngle*-1, QVector3D(1.0, 0.0, 1.0).normalized()); // Rotates the quad to show normal mapping works in all directions
        glUniformMatrix4fv(glGetUniformLocation(usingShaderProgram, "model"), 1, GL_FALSE, model.constData());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, heightMap);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }


    // draw the lamp object
    glUseProgram(lightCubeShaderProgram);
    model= QMatrix4x4();
    model.translate(lightPos);
    model.scale(QVector3D(0.1f, 0.1f, 0.1f));
    glUniformMatrix4fv(glGetUniformLocation(lightCubeShaderProgram, "model"), 1, GL_FALSE, model.constData());
    glUniformMatrix4fv(glGetUniformLocation(lightCubeShaderProgram, "view"), 1, GL_FALSE, view.constData());
    glUniformMatrix4fv(glGetUniformLocation(lightCubeShaderProgram, "proj"), 1, GL_FALSE, projection.constData());

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    calFramesPerSecond();
}


void AXBOpenGLWidget::mousePressEvent(QMouseEvent * event)
{
    makeCurrent(); // necessary
    m_camera->mousePressEvent(event);
}

void AXBOpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
    makeCurrent(); // necessary
    m_camera->mouseReleaseEvent(event);
}

void AXBOpenGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    makeCurrent(); // necessary
    m_camera->mouseMoveEvent(event);
}

void AXBOpenGLWidget::wheelEvent(QWheelEvent *event)
{
    makeCurrent(); // necessary
    m_camera->wheelEvent(event);
}

void AXBOpenGLWidget::keyPressEvent(QKeyEvent * event)
{
    makeCurrent(); // necessary
    m_camera->keyPressEvent(event);
}

void AXBOpenGLWidget::keyReleaseEvent(QKeyEvent * event)
{
    makeCurrent(); // necessary
    m_camera->keyReleaseEvent(event);
}

int AXBOpenGLWidget::getCurrentframes() const
{
    return currentframes;
}

void AXBOpenGLWidget::setRotateAngle(int newRotateAngle)
{
    rotateAngle = newRotateAngle;
    update();
}

// This function loads a texture from file. Note: texture loading functions like these are usually
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio).
// For learning purposes we'll just define it as a utility function.
GLuint AXBOpenGLWidget::loadTexture(const char *path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return textureID;
}

void AXBOpenGLWidget::RenderQuad()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
void AXBOpenGLWidget::calFramesPerSecond()
{
    ++frames;
    if(m_time.elapsed() > 1000) //每秒刷新一次
    {
        currentframes = frames;
        qDebug() << "FPS: " << frames << Qt::endl;
        frames= 0;
        m_time.start();
    }
}

