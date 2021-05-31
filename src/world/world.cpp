#include "world/world.hpp"
#include "ecs/entity.hpp"
#include "ecs/components.hpp"

Entity CreateTree(World& world, glm::vec3 pos) {
    Entity e = world.CreateEntity();
    TransformComponent& tc = e.AddComponent<TransformComponent>();
    tc.position = pos;
    tc.rotation.y = Utils::RandomFloat(0.0f, 2.0f * 3.141f);
    ModelComponent& mc = e.AddComponent<ModelComponent>();
    mc.model = world.treeModel;
    return e;
}

World::World(int size, int seed) {
    // Colors
    treeModel = new Model("resources/models/tree.obj");

    worldAmbientColor = { 0.086f, 0.174f, 0.079f };
    worldDiffuseColor = { 0.174f, 0.348f, 0.184f };
    worldSpecularColor = { 0.047f, 0.049f, 0.020f };

    sunDirection = { -0.7f, -1.0f, 0.1f };
    sunAmbientColor = { 1.0f, 1.0f, 1.0f };
    sunDiffuseColor = { 1.0f, 1.0f, 1.0f };
    sunSpecularColor = { 1.0f, 1.0f, 1.0f };

    this->size = size;
    this->cam = Camera3DPerspective{ CAMERA_TYPE_RTS, 80.0f, glm::vec3(0.0f, 10.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) };

    // Generate starting plane
    worldMesh = GenMeshPlane(size, size);
    waterMesh = GenMeshPlane(size, size);
    waterHeight = -2.0f;

    float scale = 0.0105f * 3.0f;
    int octaves = 14;
    float factor = 0.6f;

    // Generate ground map
    for (int x = 0; x < size + 1; x++) {
        for (int z = 0; z < size + 1; z++) {
            float _x = (float)x * scale;
            float _z = (float)z * scale;

            float groundHeight = 0.0f;
            float octaveHeight = 0.0f;

            for (int i = 1; i <= octaves; i++) {
                octaveHeight += (1.0f / (float)i);
                groundHeight += (1.0f / (float)i) * stb_perlin_noise3_seed(i * _x + i * 31 % 7, 0.0f, i * _z + i * 72 % 17, 0, 0, 0, seed);
            }

            groundHeight = groundHeight / octaveHeight;
            worldMesh.vertices.at(x * (size + 1) + z).posY = groundHeight * factor;
        }
    }

    scale = 0.0068f * 3.0f;
    octaves = 4;
    factor = 18.5f;

    for (int x = 0; x < size + 1; x++) {
        for (int z = 0; z < size + 1; z++) {
            float _x = (float)x * scale;
            float _z = (float)z * scale;

            float groundHeight = 0.0f;
            float octaveHeight = 0.0f;

            for (int i = 1; i <= octaves; i++) {
                octaveHeight += (1.0f / (float)i);
                groundHeight += (1.0f / (float)i) * stb_perlin_noise3_seed(i * _x + i * 31 % 7, 0.0f, i * _z + i * 72 % 17, 0, 0, 0, seed);
            }

            groundHeight = groundHeight / octaveHeight;
            if (Utils::RandomFloat() < 0.01f && groundHeight * factor > waterHeight) {
                CreateTree(*this, glm::vec3(z, groundHeight * factor, x));
            }

            //float groundHeight = stb_perlin_fbm_noise3((float)x * scale, 0.0f, (float)z * scale, lacunarity, gain, octaves) * factor;
            //float mountainHeight = stb_perlin_fbm_noise3((float)x * mountainScale, 0.0f, (float)z * mountainScale, mLacunarity, mGain, mOctaves) * mFactor;
            worldMesh.vertices.at(x * 151 + z).posY += groundHeight * factor;
        }
    }

    Utils::CaculatePerFaceNormals(worldMesh.vertices, worldMesh.indices);
    worldMesh.UpdateMesh();

    // Place trees
}

void World::Update() {
    cam.Update();
    cam.targetTarget.y = GetWorldHeight((int)cam.targetTarget.z, (int)cam.targetTarget.x);
}

void World::Draw(Shader& shader, GLFWwindow* windowHandle) {
    glUseProgram(shader.programID);
    int ww, wh; glfwGetFramebufferSize(windowHandle, &ww, &wh);
    shader.SetMat4("u_proj", cam.GetProjectionMatrix(ww, wh));
    shader.SetMat4("u_view", cam.GetViewMatrix());
    shader.SetVec3("u_viewPos", cam.cameraPos);

    shader.SetVec3("u_material.ambient", worldAmbientColor);
    shader.SetVec3("u_material.diffuse", worldDiffuseColor);
    shader.SetVec3("u_material.specular", worldSpecularColor);
    shader.SetFloat("u_material.shininess", 32.0f);

    shader.SetVec3("u_lightDirectional.direction", sunDirection);
    shader.SetVec3("u_lightDirectional.ambient", sunAmbientColor);
    shader.SetVec3("u_lightDirectional.diffuse", sunDiffuseColor);
    shader.SetVec3("u_lightDirectional.specular", sunSpecularColor);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float)size / 2.0f, 0.0f, (float)size / 2.0f));
    model = glm::scale(model, glm::vec3((float)size, 1.0f, (float)size));
    shader.SetMat4("u_model", model);
    worldMesh.Draw(shader);

    shader.SetVec3("u_material.ambient", waterAmbientColor);
    shader.SetVec3("u_material.diffuse", waterDiffuseColor);
    shader.SetVec3("u_material.specular", waterSpecularColor);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float)size / 2.0f, waterHeight, (float)size / 2.0f));
    model = glm::scale(model, glm::vec3((float)size, 1.0f, (float)size));
    shader.SetMat4("u_model", model);
    waterMesh.Draw(shader);

    // Draw all entities
    // Render entities MeshComponents & TransformComponents
    auto group = registry.group<TransformComponent>(entt::get<MeshComponent>);
    for (auto e : group) {
        const auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(e);
        shader.SetMat4("u_model", transform.GetModelMatrix());
        shader.SetVec3("u_material.ambient", mesh.ambientColor);
        shader.SetVec3("u_material.diffuse", mesh.diffuseColor);
        shader.SetVec3("u_material.specular", mesh.specularColor);

        if (mesh.mesh != nullptr) {
            if (mesh.wireframeMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            mesh.mesh->Draw(shader);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    auto models = registry.group<ModelComponent>(entt::get<TransformComponent>);
    for (auto e : models) {
        const auto& [transform, model] = models.get<TransformComponent, ModelComponent>(e);
        shader.SetMat4("u_model", transform.GetModelMatrix());
        shader.SetVec3("u_material.ambient", model.ambientColor);
        shader.SetVec3("u_material.diffuse", model.diffuseColor);
        shader.SetVec3("u_material.specular", model.specularColor);
        if (model.model != nullptr) {
            model.model->wireframe = model.wireframeMode;
            model.model->Draw(shader);
        }
    }
}

float World::GetWorldHeight(float x, float z) {
    if (x >= 0 && x <= size && z >= 0 && z <= size) {
        return worldMesh.vertices.at((int)x * (size + 1) + (int)z).posY;
    }
    else {
        return 0.0f;
    }
}

Entity World::CreateEntity() {
    entt::entity handle = registry.create();
    Entity e = { handle, this };
    entityIds.push_back(handle);
    return e;
}

Entity World::GetEntity(entt::entity e) {
    return { e, this };
}