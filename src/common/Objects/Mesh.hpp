#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>

#include "common/Shaders/shader.hpp"
#include "common/Primitives/Vertex.hpp"
#include "common/Primitives/Texture.hpp"

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