#include "TextureLoader.hpp"

#include <iostream>

GLuint TextureLoader::loadTexture2D(const std::string &path,
                                bool flipVertically,
                                GLint wrapS,
                                GLint wrapT,
                                GLint minFilter,
                                GLint magFilter)
{
    stbi_set_flip_vertically_on_load(flipVertically);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "TextureLoader: failed to load image at path: " << path << std::endl;
        return 0;
    }

    GLenum format;
    if (nrChannels == 1) {
        format = GL_RED;
    } else if (nrChannels == 3) {
        format = GL_RGB;
    } else if (nrChannels == 4) {
        format = GL_RGBA;
    } else {
        // Unexpected number of channels
        std::cerr << "TextureLoader: unexpected nrChannels = " << nrChannels
                  << " in texture " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    // Set filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // Sometimes row alignment causes issues (especially for non-4-multiple widths)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D,
                 0,            // mipmap level
                 format,       // internal format
                 width,
                 height,
                 0,            // border
                 format,       // data format
                 GL_UNSIGNED_BYTE,
                 data);

    // Generate mipmaps if the minification filter requires them
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind (optional)
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    return textureID;
}

std::pair<GLuint64, GLuint> TextureLoader::loadTexture2DBindless(const std::string &path,
                                bool flipVertically,
                                GLint wrapS,
                                GLint wrapT,
                                GLint minFilter,
                                GLint magFilter)
{
    stbi_set_flip_vertically_on_load(flipVertically);

    int width, height, nrChannels;
    std::cout << "TextureLoader: loading file " << path << std::endl;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        std::cerr << "TextureLoader: failed to load image: " << path << std::endl;
        return {0, 0};
    }

    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;
    else {
        std::cerr << "TextureLoader: unexpected nrChannels = " << nrChannels
                  << " in " << path << std::endl;
        stbi_image_free(data);
        return {0, 0};
    }
    std::cout << nrChannels << std::endl;
    GLuint textureID;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

    GLenum internalFormat;
    if (nrChannels == 1) internalFormat = GL_R8;
    else if (nrChannels == 3) internalFormat = GL_RGB8;
    else if (nrChannels == 4) internalFormat = GL_RGBA8;

    glTextureStorage2D(textureID, 1, internalFormat, width, height);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, wrapS);
    glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, wrapT);
    glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, minFilter);
    glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, magFilter);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

    if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST)
    {
        glGenerateTextureMipmap(textureID);
    }

    stbi_image_free(data);

    // Create bindless handle
    GLuint64 handle = glGetTextureHandleARB(textureID);
    glMakeTextureHandleResidentARB(handle);


    return {textureID, handle};
}

void TextureLoader::deleteTexture(GLuint texID) {
    if (texID != 0) {
        glDeleteTextures(1, &texID);
    }
}