#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

#include "stb/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model: QOpenGLFunctions_3_3_Core
{
public:
    /*  模型数据  */
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;
    /*  函数   */
    Model(std::string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }
    void Draw(GLuint shaderProgram);
private:
    /*  函数   */
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory);
};

#endif // MODEL_H
