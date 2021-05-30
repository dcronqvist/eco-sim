#include "world/world.hpp"

World::World() {

}

World::World(int size, int seed) {
    // Colors
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
            //float groundHeight = stb_perlin_fbm_noise3((float)x * scale, 0.0f, (float)z * scale, lacunarity, gain, octaves) * factor;
            //float mountainHeight = stb_perlin_fbm_noise3((float)x * mountainScale, 0.0f, (float)z * mountainScale, mLacunarity, mGain, mOctaves) * mFactor;
            worldMesh.vertices.at(x * 151 + z).posY += groundHeight * factor;
        }
    }

    Utils::CaculatePerFaceNormals(worldMesh.vertices, worldMesh.indices);
    worldMesh.UpdateMesh();
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
}

float World::GetWorldHeight(float x, float z) {
    if (x >= 0 && x <= size && z >= 0 && z <= size) {
        return worldMesh.vertices.at((int)x * (size + 1) + (int)z).posY;
    }
    else {
        return 0.0f;
    }
}