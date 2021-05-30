#include "utils/input.hpp"

#define INITKEYPRESS(window, key) Input::currentKeyState.insert(GetKeyPressPair(window, key));
#define INITMOUSEPRESS(window, button) Input::currentMouseState.insert(GetMousePressPair(window, button));

std::map<int, bool> Input::currentKeyState = {};
std::map<int, bool> Input::previousKeyState = {};
std::map<int, bool> Input::currentMouseState = {};
std::map<int, bool> Input::previousMouseState = {};
glm::vec2 Input::currentMousePos = {};
float Input::currentMouseWheelValue = 0;
float Input::previousMouseWheelValue = 0;

std::pair<int, bool> GetKeyPressPair(GLFWwindow* window, int key) {
    return std::pair<int, bool>(key, glfwGetKey(window, key) == GLFW_PRESS);
}

std::pair<int, bool> GetMousePressPair(GLFWwindow* window, int button) {
    return std::pair<int, bool>(button, glfwGetMouseButton(window, button) == GLFW_PRESS);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input::currentMouseWheelValue += yoffset;
}

void Input::Init(GLFWwindow* window) {
    glfwSetScrollCallback(window, ScrollCallback);
}

void Input::Begin(GLFWwindow* window) {
    Input::currentKeyState = {};
    for (int i = 32; i < 350; i++) {
        if (glfwGetKeyScancode(i) != -1) {
            INITKEYPRESS(window, i);
        }
    }
    Input::currentMouseState = {};
    for (int i = 0; i < 8; i++) {
        INITMOUSEPRESS(window, i);
    }

    double mx, my;
    glfwGetCursorPos(window, &mx, &my);
    Input::currentMousePos = glm::vec2((float)mx, (float)my);
}

void Input::End() {
    Input::previousKeyState = Input::currentKeyState;
    Input::previousMouseState = Input::currentMouseState;
    Input::previousMouseWheelValue = Input::currentMouseWheelValue;
}

bool Input::IsKeyDown(int key) {
    return Input::currentKeyState.at(key);
}

bool Input::IsKeyUp(int key) {
    return !Input::currentKeyState.at(key);
}

bool Input::IsKeyPressed(int key) {
    return Input::currentKeyState.at(key) && !Input::previousKeyState.at(key);
}

bool Input::IsMouseButtonDown(int button) {
    return Input::currentMouseState.at(button);
}

bool Input::IsMouseButtonUp(int button) {
    return !Input::currentMouseState.at(button);
}

bool Input::IsMouseButtonPressed(int button) {
    return Input::currentMouseState.at(button) && !Input::previousMouseState.at(button);
}

glm::vec2 Input::GetMousePosition() {
    return Input::currentMousePos;
}

float Input::GetMouseWheelMove() {
    return Input::currentMouseWheelValue - Input::previousMouseWheelValue;
}