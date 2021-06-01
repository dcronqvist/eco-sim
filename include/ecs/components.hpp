#pragma once

#include "glm/glm.hpp"
#include "imgui.h"
#include "models/mesh.hpp"
#include "models/meshgen.hpp"
#include "models/model.hpp"

struct Component {
    virtual void UI() = 0;
};

struct TransformComponent : public Component {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    TransformComponent() {
        scale = { 1.0f, 1.0f, 1.0f };
        rotation = { 0.0f, 0.0f, 0.0f };
        position = { 0.0f, 0.0f, 0.0f };
    }

    glm::mat4 GetModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

    void UI() {
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", glm::value_ptr(position), 0.2f, -1000.0f, 1000.0f, "%.2f", 0);
        ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.2f, -1000.0f, 1000.0f, "%.2f", 0);
        ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.2f, -1000.0f, 1000.0f, "%.2f", 0);
    }
};

struct MeshComponent : public Component {
    Mesh* mesh;
    bool wireframeMode;

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    MeshComponent() {
        mesh = nullptr;
        wireframeMode = false;

        ambientColor = { 1.0f, 1.0f, 1.0f };
        diffuseColor = { 1.0f, 1.0f, 1.0f };
        specularColor = { 0.15f, 0.15f, 0.15f };

    }

    void UI() {
        ImGui::Text("Mesh");
        if (mesh != nullptr) {
            ImGui::Text("Vertex Amount: %d", mesh->vertices.size());
            ImGui::Text("Face Amount: %d", mesh->indices.size() / 3);
            ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientColor));
            ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(diffuseColor));
            ImGui::ColorEdit3("Specular Color", glm::value_ptr(specularColor));
            ImGui::Checkbox("Wireframe Mode", &wireframeMode);
        }
        else {
            ImGui::Text("No Mesh Attached.");
            if (ImGui::Button("Attach Plane Mesh")) {
                mesh = new Mesh(GenMeshPlane(5, 5));
            }
        }
    }
};

struct ModelComponent : public Component {
    Model* model;
    bool wireframeMode;

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    ModelComponent() {
        model = nullptr;
        ambientColor = { 1.0f, 1.0f, 1.0f };
        diffuseColor = { 1.0f, 1.0f, 1.0f };
        specularColor = { 0.15f, 0.15f, 0.15f };
        wireframeMode = false;
    }

    void UI() {
        ImGui::Text("Model");
        if (model != nullptr) {
            ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientColor));
            ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(diffuseColor));
            ImGui::ColorEdit3("Specular Color", glm::value_ptr(specularColor));
            ImGui::Checkbox("Wireframe Mode", &(wireframeMode));
        }
        else {
            if (ImGui::Button("Load Tree Model")) {
                model = new Model("resources/models/tree.obj");
            }
        }
    }
};