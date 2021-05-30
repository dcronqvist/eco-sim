#pragma once

#include "glad.h"
#include "glfw3.h"
#include "shaders/shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
using namespace std;

struct Vertex {
    float posX;
    float posY;
    float posZ;

    float normX;
    float normY;
    float normZ;

    float texX;
    float texY;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

private:
    unsigned int VAO, VBO, EBO;

public:
    Mesh();
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader& shader);
    void UpdateMesh();
    ~Mesh();

private:
    void SetupMesh();
};