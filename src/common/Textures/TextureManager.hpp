#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <iostream>
#include <utility>

#include <common/Textures/TextureLoader.hpp>

class TextureManager {
    public:
        TextureManager() = default;
        ~TextureManager();

        
        /// @brief Retrieves the requested texture, but will check if this texture is not already loaded
        /// @param path File to texture
        /// @param flipVertically flip Texture vertically if true (needed for OpenGL)
        /// @param wrapS 
        /// @param wrapT 
        /// @param minFilter 
        /// @param magFilter 
        /// @return bindless texture handle id
        std::pair<GLuint64, GLuint> GetTexture2D(const std::string& path,
                                bool flipVertically = true,
                                GLint wrapS = GL_REPEAT,
                                GLint wrapT = GL_REPEAT,
                                GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                                GLint magFilter = GL_LINEAR);

        /// @brief Clears all stored textures and destroys them from the OpenGL context
        void Clear();

    private:
        /// @brief Contains all already loaded textures by their pathname
        std::unordered_map<std::string, std::pair<GLuint64, GLuint>> textureMap;
};

#endif