#include "models/mesh.hpp"

Mesh::Mesh() {
    this->vertices = {};
    this->indices = {};
    this->textures = {};
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->VAO = 0;
    this->VBO = 0;
    this->EBO = 0;
    this->SetupMesh();
}

void Mesh::UpdateMesh() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(Vertex), &vertices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::SetupMesh() {
    // Create VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // Create VBO & bind
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Fill VBO with vertices
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Positions
    int posOffset = offsetof(Vertex, posX);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, posX));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normX));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texX));
    glEnableVertexAttribArray(2);

    // Create EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Fill EBO with indices
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    // Assign vertex attrib pointers


    // Unbind VAO
    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader) {
    glUseProgram(shader.programID);

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    shader.SetBool("u_textures_present", textures.size() == 0);
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        string number;
        string name = textures.at(i).type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        }
        else if (name == "texture_specular") {
            number = std::to_string(specularNr++);
        }

        shader.SetFloat((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures.at(i).id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

Mesh::~Mesh() {
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
}