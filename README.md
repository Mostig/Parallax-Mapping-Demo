# Parallax-Mapping-Demo
It's my CG homework, a demo of Parallax Mapping developed with Qt+openGL.

<img src="https://gitee.com/mostig/csdn-image/raw/master/data/image-20220501162151810.png" alt="image-20220501162151810" style="zoom: 25%;" />

Comparison of  (1)diffuse map, (2)diffuse map + normal map and (3)diffuse map + normal map + height map:

<img src="https://gitee.com/mostig/csdn-image/raw/master/data/image-20230708152248166.png" alt="image-20230708152248166" style="zoom: 30%;" />

Comparison of (1)Parallax Mapping, (2)Steep Parallax Mapping and (3)Parallax Occlusion Mapping: 

<img src="https://gitee.com/mostig/csdn-image/raw/master/data/image-20230708153731860.png" alt="image-20230708153731860" style="zoom: 33%;" />



### 环境

#### 硬件环境

系统： Windows 10 x64

#### 软件环境

开发环境： QT Creator 6.0.2 + MinGW 8.1.0(64bit) + OpenGL3.3 core
程序开发语言：C++

#### 第三方库

Assimp 5.2.3 ：基于Assimp库实现加载网格模型
stb_image ：图像解码库

### 功能

- 点击“凹凸映射 Demo”按钮：加载凹凸映射Demo

  - 法线贴图(Normal Mapping)

  - 视差贴图(Parallax Mapping)

  - 陡峭视差映射(Steep Parallax Mapping)

  - 视差遮蔽映射(Parallax Occlusion Mapping)

- 读取obj格式的mesh文件(支持漫反射贴图、光照贴图和法线贴图)
- 交互
  - 拖动滑块控制模型旋转
  - 控制相机移动
    - 【W】【S】【A】【D】【Shift】【Space】进行前、后、左、右、上、下平移
    - 鼠标滚轮zoom in/out
    - 鼠标右键拖动视角



详见“说明文档.pdf”
