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

void TextureLoader::deleteTexture(GLuint texID) {
    if (texID != 0) {
        glDeleteTextures(1, &texID);
    }
}