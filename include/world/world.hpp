#pragma once
#include "models/mesh.hpp"
#include "models/meshgen.hpp"
#include "utils/stb_perlin.h"
#include "utils/utility.hpp"
#include "cameras/camera3d_perspective.hpp"

class World {
public:
    Camera3DPerspective cam;
    Mesh worldMesh;
    int size;

    World();
    World(int size, int seed);

    void Update();
    void Draw(Shader& shader);
    float GetWorldHeight(float x, float z);
};