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
#include "world/world.hpp"
#include "ecs/entity.hpp"
#include "ecs/components.hpp"

// Template stuff
Shader s;
World world;

entt::entity selectedEntity = entt::null;

glm::vec3 slopeColor = { 1.0f, 1.0f, 1.0f };
float slopeFactor = 1.0f;
float sloping = 1.0f;

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

    glEnable(GL_DEPTH_TEST);
    Input::Init(this->windowHandle);

    world = World(150, 1245125);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void GameWindow::Update() {
    Input::Begin(this->windowHandle);

    world.Update();
    // Performs hot-reload of shader. Only reloads whenever it has been modified - so not every frame.
    s.ReloadFromFile();
    s.SetVec3("u_slopeColor", slopeColor);
    s.SetFloat("u_slopeFactor", slopeFactor);
    s.SetFloat("u_sloping", sloping);

    if (Input::IsKeyPressed(GLFW_KEY_A)) {
        world.CreateEntity();
    }

    Input::End();
}

void SubmitUI() {
    ImGui::Begin("Variables", NULL, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::SliderFloat("FOV", &(world.cam.fov), 1.0f, 120.0f, "%.0f", 0);
    ImGui::SliderFloat("Lerp", &(world.cam.velocityLerpFactor), 0.0f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Distance", &(world.cam.targetTargetDistance), 1.0f, 100.0f, "%.0f", 0);
    ImGui::SliderFloat("Panning Speed", &(world.cam.panningSpeed), 0.0f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);
    ImGui::SliderFloat("Zooming Speed", &(world.cam.zoomingSpeed), 0.0f, 10.0f, "%.2f", ImGuiSliderFlags_Logarithmic);

    ImGui::Separator();

    ImGui::SliderAngle("Pitch", &(world.cam.targetPitch), -360.0f, 360.0f, "%.1f deg", 0);
    ImGui::SliderAngle("Yaw", &(world.cam.targetYaw), -360.0f, 360.0f, "%.1f deg", 0);
    ImGui::SliderFloat3("Camera Target", glm::value_ptr(world.cam.targetTarget), -100.0f, 100.0f, "%.1f", 0);

    ImGui::Separator();

    ImGui::SliderFloat("Water Height", &(world.waterHeight), -10.0f, 10.0f, "%.1f", 0);
    ImGui::ColorEdit3("Water Ambient Color", glm::value_ptr(world.waterAmbientColor), 0);
    ImGui::ColorEdit3("Water Diffuse Color", glm::value_ptr(world.waterDiffuseColor), 0);
    ImGui::ColorEdit3("Water Specular Color", glm::value_ptr(world.waterSpecularColor), 0);

    ImGui::Separator();

    ImGui::ColorEdit3("Slope Color", glm::value_ptr(slopeColor), 0);
    ImGui::SliderFloat("Slope Factor", &slopeFactor, 0.0f, 50.0f, "%.2f", 0);
    ImGui::SliderFloat("Sloping", &sloping, 0.0f, 10.0f, "%.2f", 0);

    ImGui::End();

    if (Input::IsKeyDown(GLFW_KEY_S)) {
        int x = 2;
    }

    // Entities
    ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::ListBoxHeader("Entities");
    for (entt::entity e : world.entityIds) {
        string name = to_string((uint32_t)e);
        if (ImGui::Selectable(name.c_str(), selectedEntity == e)) {
            selectedEntity = e;
        }
    }
    ImGui::ListBoxFooter();

    // Get all components of entity

    if (world.registry.valid(selectedEntity)) {
        Entity e = world.GetEntity(selectedEntity);

        if (ImGui::Button("Add TransformComponent")) {
            e.AddComponent<TransformComponent>();
        }
        if (ImGui::Button("Add MeshComponent")) {
            e.AddComponent<MeshComponent>();
        }
        if (ImGui::Button("Add ModelComponent")) {
            e.AddComponent<ModelComponent>();
        }
        if (ImGui::Button("Add FollowGroundHeightComponent")) {
            e.AddComponent<FollowGroundHeightComponent>();
        }

        ImGui::Separator();

        if (e.HasComponent<TransformComponent>()) {
            e.GetComponent<TransformComponent>().UI();
            ImGui::Separator();
        }
        if (e.HasComponent<MeshComponent>()) {
            e.GetComponent<MeshComponent>().UI();
            ImGui::Separator();
        }
        if (e.HasComponent<ModelComponent>()) {
            e.GetComponent<ModelComponent>().UI();
            ImGui::Separator();
        }
        if (e.HasComponent<FollowGroundHeightComponent>()) {
            e.GetComponent<FollowGroundHeightComponent>().UI();
            ImGui::Separator();
        }
    }

    ImGui::End();
}

void GameWindow::Render() {
    // Clear the window
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create new imgui frames
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    world.Draw(s, this->windowHandle);

    SubmitUI();

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