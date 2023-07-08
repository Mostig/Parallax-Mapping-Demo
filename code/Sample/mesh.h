#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
    // tangent
    QVector3D Tangent;
    // bitangent
    QVector3D Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


struct Material{
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float shininess;
};

class Mesh: QOpenGLFunctions_3_3_Core
{
public:
    /*  网格数据  */
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material material;
    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material);
    Mesh(const Mesh& p);
    Mesh& operator=(const Mesh& p);
    void Draw(GLuint shaderProgram);

private:
    /*  渲染数据  */
    unsigned int VBO, EBO;
    unsigned int uniformBlockIndex;
;
    /*  函数  */
    void setupMesh();

};

#endif // MESH_H
