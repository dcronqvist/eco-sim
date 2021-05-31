#include "models/meshgen.hpp"
#include "utils/utility.hpp"

Mesh GenMeshPlane(int xres, int zres) {
    float xstep = 1.0f / (float)xres;
    float zstep = 1.0f / (float)zres;

    vector<Vertex> vertices;

    // Calculate all available positions
    for (int z = 0; z < zres + 1; z++) {
        for (int x = 0; x < xres + 1; x++) {
            Vertex v;
            v.posX = x * xstep - 0.5f;
            v.posY = 0.0f;
            v.posZ = z * zstep - 0.5f;
            v.texX = x * xstep;
            v.texY = z * zstep;
            vertices.push_back(v);
        }
    }

    vector<unsigned int> indices;
    for (unsigned int i = 0; i < ((xres + 1) * zres) - 1; i++) {
        int skip = (i / xres) * (xres + 1) - 1;
        if (i == skip) {
            continue;
        }

        // First tri
        indices.push_back(i);
        indices.push_back(i + xres + 1);
        indices.push_back(i + xres + 2);

        // Second tri
        indices.push_back(i);
        indices.push_back(i + xres + 2);
        indices.push_back(i + 1);
    }

    Utils::CaculatePerFaceNormals(vertices, indices);

    Texture tex;
    tex.id = TextureFromFile("one.jpg", "resources/models");
    tex.path = "";
    tex.type = "texture_diffuse";

    return Mesh(vertices, indices, { tex });
}