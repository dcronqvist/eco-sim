#include "display/game_window.hpp"
#include "shaders/shader.hpp"
#include "utils/input.hpp"
#include "cameras/camera3d_perspective.hpp"
#include "models/model.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "models/meshgen.hpp"
#include "utils/utility.hpp"
#include "utils/stb_perlin.h"

// Template stuff
Shader s;
unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

// matrices
glm::mat4 model;
float angle = 0.0f;

// custom camera
Camera3DPerspective cam;

// Colors
glm::vec3 objDiffCol = { 1.0f, 1.0f, 1.0f };
glm::vec3 objAmbientCol = { 0.15f, 0.15f, 0.15f };
glm::vec3 objSpecCol;

glm::vec3 sunDiffCol = { 1.0f, 1.0f, 1.0f };
glm::vec3 sunAmbientCol = { 1.0f, 1.0f, 1.0f };
glm::vec3 sunSpecCol = { 1.0f, 1.0f, 1.0f };

// light
glm::vec3 lightPos = { -0.1f, -1.0f, 0.1f };

Model backpack;
Mesh plane;

// Ground variables
int octaves = 1;
float factor = 0.1f;
float falloffFactor = 1.0f;
float scale = 0.2f;
float power = 1.0f;
bool wireframe = false;
int seed = 12401241;

// Called whenever the window or framebuffer's size is changed
void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// 1. The first thing that is run when starting the window
void GameWindow::Initialize() {
    // Set GLFW stuff before window is created
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    cam = Camera3DPerspective{ CAMERA_TYPE_RTS, 80.0f, glm::vec3(0.0f, 10.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };
}

// 2. Run after the window has been created, as well as the OpenGL context
void GameWindow::LoadContent() {
    // Set callback
    glfwSetFramebufferSizeCallback(this->windowHandle, FramebufferSizeCallback);

    // Initialize imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->windowHandle, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    std::cout << "INFO::IMGUI::SUCCESSFULLY_INITIALIZED" << std::endl;

    // Load the template shader
    s = Shader::LoadShader("resources/shaders/basic_lighting.vs", "resources/shaders/basic_lighting.fs");

    //backpack = Model("resources/models/backpack.obj");
    std::cout << "INFO::MODEL::SUCCESSFULLY_INITIALIZED" << std::endl;

    glEnable(GL_DEPTH_TEST);
    Input::Init(this->windowHandle);
    plane = GenMeshPlane(150, 150);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void GameWindow::Update() {
    Input::Begin(this->windowHandle);

    Utils::CaculatePerFaceNormals(plane.vertices, plane.indices);
    plane.UpdateMesh();

    cam.Update();

    // Performs hot-reload of shader. Only reloads whenever it has been modified - so not every frame.
    s.ReloadFromFile();

    Input::End();
}

void GameWindow::Render() {
    // Clear the window
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create new imgui frames
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glUseProgram(s.programID);

    int ww, wh; glfwGetFramebufferSize(this->windowHandle, &ww, &wh);
    s.SetMat4("u_proj", cam.GetProjectionMatrix(ww, wh));
    s.SetMat4("u_view", cam.GetViewMatrix());
    s.SetVec3("u_viewPos", cam.cameraPos);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    s.SetMat4("u_model", model);

    /*
    struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    struct Light {
        vec3 position;
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    };

    uniform Material u_material;
    uniform Light u_lightDirectional;
    */
    s.SetVec3("u_material.ambient", objAmbientCol);
    s.SetVec3("u_material.diffuse", objDiffCol);
    s.SetVec3("u_material.specular", objSpecCol);
    s.SetFloat("u_material.shininess", 32.0f);

    s.SetVec3("u_lightDirectional.direction", lightPos);
    s.SetVec3("u_lightDirectional.ambient", sunAmbientCol);
    s.SetVec3("u_lightDirectional.diffuse", sunDiffCol);
    s.SetVec3("u_lightDirectional.specular", sunSpecCol);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
    s.SetMat4("u_model", model);
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    plane.Draw(s);
    // Draw imgui

    ImGui::Begin("Variables", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SliderFloat("FOV", &(cam.fov), 1.0f, 120.0f, "%.0f", 0);
    ImGui::SliderFloat("Lerp", &(cam.velocityLerpFactor), 0.0f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Distance", &(cam.targetTargetDistance), 1.0f, 100.0f, "%.0f", 0);
    ImGui::SliderFloat("Panning Speed", &(cam.panningSpeed), 0.0f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Zooming Speed", &(cam.zoomingSpeed), 0.0f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);

    ImGui::Separator();

    ImGui::SliderAngle("Pitch", &(cam.targetPitch), -360.0f, 360.0f, "%.1f deg", 0);
    ImGui::SliderAngle("Yaw", &(cam.targetYaw), -360.0f, 360.0f, "%.1f deg", 0);

    ImGui::SliderFloat3("Camera Target", glm::value_ptr(cam.targetTarget), -100.0f, 100.0f, "%.1f", 0);

    ImGui::End();

    ImGui::Begin("World Variables", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Ground");
    ImGui::InputInt("Seed", &seed, 1, 100, 0);
    ImGui::DragFloat("Factor", &factor, 0.001f, 0.0f, 1.0f, "%.2f", 0);
    ImGui::DragFloat("Falloff Factor", &falloffFactor, 0.1f, 0.0f, 3.0f, "%.2f", 0);
    ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderInt("Octaves", &octaves, 1, 50, "%d", 0);
    ImGui::SliderFloat("Power", &power, 0.01f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);

    if (ImGui::Button("Generate")) {
        for (int x = 0; x < 151; x++) {
            for (int z = 0; z < 151; z++) {
                float _x = (float)x * scale;
                float _z = (float)z * scale;

                float groundHeight = 0.0f;
                float octaveHeight = 0.0f;

                for (int i = 1; i <= octaves; i++) {
                    octaveHeight += (1.0f / (float)i);
                    groundHeight += (1.0f / (float)i) * stb_perlin_noise3_seed(i * _x + i * 31 % 7, 0.0f, i * _z + i * 72 % 17, 0, 0, 0, seed);
                }

                groundHeight = groundHeight / octaveHeight;
                groundHeight = powf(groundHeight, power);
                //float groundHeight = stb_perlin_fbm_noise3((float)x * scale, 0.0f, (float)z * scale, lacunarity, gain, octaves) * factor;
                //float mountainHeight = stb_perlin_fbm_noise3((float)x * mountainScale, 0.0f, (float)z * mountainScale, mLacunarity, mGain, mOctaves) * mFactor;
                plane.vertices.at(x * 151 + z).posY = groundHeight * factor;
            }
        }
    }

    ImGui::End();

    ImGui::Begin("Object Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::ColorPicker3("Object Ambient Color", glm::value_ptr(objAmbientCol), ImGuiColorEditFlags_NoSidePreview);
    ImGui::ColorPicker3("Object Diffuse Color", glm::value_ptr(objDiffCol), ImGuiColorEditFlags_NoSidePreview);
    ImGui::ColorPicker3("Object Specular Color", glm::value_ptr(objSpecCol), ImGuiColorEditFlags_NoSidePreview);
    //ImGui::ColorPicker3("Light Color", glm::value_ptr(lightCol), ImGuiColorEditFlags_NoPicker);
    ImGui::SliderFloat("Model Angle", &angle, 0.0f, 2.0f * 3.141f, "%.2f", 0);
    //
    ImGui::Checkbox("Wireframe", &wireframe);

    ImGui::End();

    ImGui::Begin("Sun Properties", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::ColorPicker3("Sun Ambient Color", glm::value_ptr(sunAmbientCol), ImGuiColorEditFlags_NoSidePreview);
    ImGui::ColorPicker3("Sun Diffuse Color", glm::value_ptr(sunDiffCol), ImGuiColorEditFlags_NoSidePreview);
    ImGui::ColorPicker3("Sun Specular Color", glm::value_ptr(sunSpecCol), ImGuiColorEditFlags_NoSidePreview);

    ImGui::SliderFloat3("Light Direction", glm::value_ptr(lightPos), -1.0f, 1.0f, "%.1f", 0);

    ImGui::End();

    // Render imgui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap double buffers and poll OS-events
    glfwSwapBuffers(this->windowHandle);
    glfwPollEvents();
}

void GameWindow::Unload() {
    // Destroy imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}