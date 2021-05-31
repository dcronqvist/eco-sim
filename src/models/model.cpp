#include "models/model.hpp"
#include "utils/stb_image.h"
#include "models/OBJ_Loader.h"

Model::Model() {

}

unsigned int TextureFromFile(const char* path, const string& directory) {
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::Draw(Shader& shader) {
    if (this->wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Model::LoadModel(string path) {
    // Assimp::Importer import;
    // const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // this->meshes = vector<Mesh>(scene->mNumMeshes);
    // if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    //     cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
    //     return;
    // }

    // directory = path.substr(0, path.find_last_of('/'));

    // cout << "PROCESSING ROOT NOTE" << endl;
    // ProcessNode(scene->mRootNode, scene);

    objl::Loader loader;
    directory = path.substr(0, path.find_last_of('/'));
    bool success = loader.LoadFile(path);
    if (success) {
        this->meshes = vector<Mesh>(loader.LoadedMeshes.size());
        for (int i = 0; i < loader.LoadedMeshes.size(); i++) {
            objl::Mesh curMesh = loader.LoadedMeshes[i];

            vector<Vertex> vertices = {};
            for (int j = 0; j < curMesh.Vertices.size(); j++) {
                objl::Vertex ov = curMesh.Vertices[j];
                Vertex vertex = { ov.Position.X, ov.Position.Y, ov.Position.Z,
                                  ov.Normal.X, ov.Normal.Y, ov.Normal.Z,
                                  ov.TextureCoordinate.X, ov.TextureCoordinate.Y };
                vertices.push_back(vertex);
            }

            vector<unsigned int> indices = {};
            for (int j = 0; j < curMesh.Indices.size(); j++) {
                indices.push_back(curMesh.Indices[j]);
            }

            string diffuseTextureFile = curMesh.MeshMaterial.map_Kd;
            string specularTextureFile = curMesh.MeshMaterial.map_Ks;

            vector<Texture> textures = {};

            bool skip = false;
            for (unsigned int j = 0; j < loadedTextures.size(); j++) {
                if (std::strcmp(loadedTextures.at(j).path.data(), diffuseTextureFile.c_str()) == 0) {
                    textures.push_back(loadedTextures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                Texture diffTex;
                unsigned int diffId = TextureFromFile(diffuseTextureFile.c_str(), this->directory);
                diffTex.id = diffId;
                diffTex.path = diffuseTextureFile;
                diffTex.type = "texture_diffuse";
                textures.push_back(diffTex);
                loadedTextures.push_back(diffTex);
            }

            skip = false;
            for (unsigned int j = 0; j < loadedTextures.size(); j++) {
                if (std::strcmp(loadedTextures.at(j).path.data(), specularTextureFile.c_str()) == 0) {
                    textures.push_back(loadedTextures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                Texture specTex;
                unsigned int specId = TextureFromFile(specularTextureFile.c_str(), this->directory);
                specTex.id = specId;
                specTex.path = specularTextureFile;
                specTex.type = "texture_specular";
                textures.push_back(specTex);
                loadedTextures.push_back(specTex);
            }

            Mesh m = Mesh(vertices, indices, textures);
            this->meshes.at(i) = m;
            vertices.clear();
            indices.clear();
            textures.clear();
        }
    }
}