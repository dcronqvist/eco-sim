#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include "glad.h"
#include "glfw3.h"
#include "models/mesh.hpp"
#include <vector>
using namespace std;

namespace Utils {

    extern float deltaTime;
    extern float previousTime;

    bool ReadFile(std::string file, std::string& fileContents, bool addLineTerminator = false);
    long GetFileModTime(std::string file);
    float Clamp(float value, float min, float max);
    void CaculatePerFaceNormals(vector<Vertex>& vertices, vector<unsigned int>& indices);
};