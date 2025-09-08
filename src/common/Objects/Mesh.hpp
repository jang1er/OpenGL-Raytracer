#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>

#include "common/Shaders/shader.hpp"

enum TextureType {
    DIFFUSE,
    SPECULAR
};

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    TextureType type;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

    private:
        // render Data
        unsigned int VAO, VBO, EBO;

        void setupMesh();
};
#endif