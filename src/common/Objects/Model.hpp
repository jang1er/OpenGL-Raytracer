#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <common/Shaders/shader.hpp>
#include <common/Objects/Mesh.hpp>

class Model {
    public:
        Model(std::string path){
            loadModel(path);
        }
        void Draw(Shader &shader);

    private:
        //model data
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

#endif