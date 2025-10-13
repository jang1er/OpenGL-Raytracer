#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <common/Shaders/shader.hpp>
#include <common/Objects/Mesh.hpp>

#include <common/Textures/TextureManager.hpp>

class Model {
    public:
        Model(std::shared_ptr<TextureManager> texManager, std::string path) : textureManager(std::move(texManager))
        {
            loadModel(path);
        }
        void Draw(Shader &shader);

    private:
        //model data
        std::vector<Mesh> meshes;
        std::string directory;
        std::shared_ptr<TextureManager> textureManager;

        /// @brief 
        /// @param path 
        void loadModel(std::string path);

        /// @brief 
        /// @param node 
        /// @param scene 
        void processNode(aiNode* node, const aiScene* scene);

        /// @brief 
        /// @param mesh 
        /// @param scene 
        /// @return 
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        /// @brief 
        /// @param mat 
        /// @param type 
        /// @param typeName 
        /// @return 
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
};

#endif