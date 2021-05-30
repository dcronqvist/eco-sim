#pragma once

#include "glad.h"
#include "glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class Input {
    public:
    static std::map<int, bool> currentKeyState;
    static std::map<int, bool> previousKeyState;
    static std::map<int, bool> currentMouseState;
    static std::map<int, bool> previousMouseState;
    static glm::vec2 currentMousePos;
    static float currentMouseWheelValue;
    static float previousMouseWheelValue;

    static void Init(GLFWwindow* window);
    static void Begin(GLFWwindow* windowHandle);
    static void End();
    static bool IsKeyDown(int key);
    static bool IsKeyUp(int key);
    static bool IsKeyPressed(int key);

    static bool IsMouseButtonDown(int button);
    static bool IsMouseButtonUp(int button);
    static bool IsMouseButtonPressed(int button);
    static glm::vec2 GetMousePosition();
    static float GetMouseWheelMove();
};