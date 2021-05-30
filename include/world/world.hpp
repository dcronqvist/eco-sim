#pragma once
#include "models/mesh.hpp"
#include "models/meshgen.hpp"
#include "utils/stb_perlin.h"
#include "utils/utility.hpp"

class World {
public:
    Mesh worldMesh;
    int size;

    World();
    World(int size, int seed);

    void Draw(Shader& shader);
    float GetWorldHeight(float x, float z);
};