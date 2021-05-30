#pragma once

#include "glad.h"
#include "glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include "models/mesh.hpp"
using namespace std;

class Model {
private:
    vector<Mesh> meshes;
    vector<Texture> loadedTextures;
    string directory;

public:
    bool wireframe;

    Model();
    Model(const char* path) {
        this->wireframe = false;
        cout << "ATTEMPTING MODEL LOADING" << endl;
        LoadModel(path);
    }
    void Draw(Shader& shader);

private:
    void LoadModel(string path);
};

// TODO: Move this to a textures file instead
unsigned int TextureFromFile(const char* path, const string& directory);