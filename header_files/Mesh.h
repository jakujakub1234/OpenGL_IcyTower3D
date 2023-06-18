/*
The code template was taken from https://learnopengl.com/
*/

#pragma once

#include "ShaderManager.h"

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    int m_bone_ids[MAX_BONE_INFLUENCE];
    float m_weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    char* type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    unsigned int VAO;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(ShaderManager& shaderManager);
private:
    unsigned int VBO, EBO;

    void SetupMesh();
};