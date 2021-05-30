#pragma once

#include "glad.h"
#include "glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>


class Shader {
public:
    unsigned int programID;
    std::string vertexFile;
    std::string fragmentFile;

    long fragmentModTimeOnLoad;

    Shader();
    void Unload();
    void ReloadFromFile();
    static Shader LoadShader(std::string fileVertexShader, std::string fileFragmentShader);

public:
    void SetVec3(const char* uniform, glm::vec3 value);
    void SetFloat(const char* uniform, float value);
    void SetMat4(const char* uniform, glm::mat4 value);
    void SetBool(const char* uniform, bool value);

private:
    static bool CompileShader(unsigned int shaderId, char(&infoLog)[512]);
    static bool LinkProgram(unsigned int programID, char(&infoLog)[512]);
};