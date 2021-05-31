#pragma once
#include "models/mesh.hpp"
#include "models/meshgen.hpp"
#include "utils/stb_perlin.h"
#include "utils/utility.hpp"
#include "cameras/camera3d_perspective.hpp"
#include "ecs/entt.hpp"

class Entity;

class World {
    public:
    // World camera
    Camera3DPerspective cam;

    // World mesh
    Mesh worldMesh;
    Mesh waterMesh;
    // Size of world
    int size;
    float waterHeight;

    // Water ambient color
    glm::vec3 waterAmbientColor;
    // Water diffuse color
    glm::vec3 waterDiffuseColor;
    // Water specular color
    glm::vec3 waterSpecularColor;

    // World ambient color
    glm::vec3 worldAmbientColor;
    // Wirld diffuse color
    glm::vec3 worldDiffuseColor;
    // World Specular color
    glm::vec3 worldSpecularColor;

    // Direction which the sun is pointing towards
    glm::vec3 sunDirection;
    // Sun's ambient color
    glm::vec3 sunAmbientColor;
    // Sun's diffuse color
    glm::vec3 sunDiffuseColor;
    // Sun's specular color
    glm::vec3 sunSpecularColor;

    // ECS Stuff
    vector<entt::entity> entityIds;
    entt::registry registry;

    Model* treeModel;

    World() = default;
    World(int size, int seed);

    void Update();
    void Draw(Shader& shader, GLFWwindow* windowHandle);
    float GetWorldHeight(float x, float z);
    Entity CreateEntity();
    Entity GetEntity(entt::entity e);
};