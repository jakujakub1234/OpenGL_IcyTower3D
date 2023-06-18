/*
The code template was taken from https://learnopengl.com/
*/

#pragma once

#include "Mesh.h"
#include "ShaderManager.h"

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

class Model
{
public:
    std::vector<Texture> textures_loaded;

    Model();
    Model(char* path);
    void Draw(ShaderManager& shaderManager);
private:
    // model data
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, char* typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};