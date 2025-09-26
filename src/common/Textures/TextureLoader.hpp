#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include "stb_image.h"
#include <string>
#include <glad/glad.h>

class TextureLoader{
    public:
    /// @brief loads an 2D image texture from the provided path
    /// @param path path to image
    /// @param flipVertically flipt the image vertically
    /// @param wrapS OpenGL wrap 
    /// @param warpT OpenGL wrap 
    /// @param minFilter 
    /// @param magFilter 
    /// @return texture ID of loaded image
    static GLuint loadTexture2D(const std::string &path,
                                bool flipVertically = true,
                                GLint wrapS = GL_REPEAT,
                                GLint wrapT = GL_REPEAT,
                                GLint minFilter = GL_LINEAR_MIPMAP_LINEAR,
                                GLint magFilter = GL_LINEAR);

    /// @brief deletes the texture
    /// @param textID Texture to delete
    static void deleteTexture(GLuint textID);
};


#endif