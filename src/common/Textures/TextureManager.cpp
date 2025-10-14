#include <common/Textures/TextureManager.hpp>

std::pair<GLuint64, GLuint> TextureManager::GetTexture2D(const std::string& path,
                                bool flipVertically,
                                GLint wrapS,
                                GLint wrapT,
                                GLint minFilter,
                                GLint magFilter){
    // lookup in texture map
    auto iterator = textureMap.find(path);
    if(iterator != textureMap.end()){
        //std::cout << "Texture is already loaded" << std::endl;
        return iterator->second;
    }
    std::cout << "Loading texture from file" << std::endl;
    // Load the texture from File
    auto textureID = TextureLoader::loadTexture2DBindless(path, flipVertically, wrapS, wrapT,  minFilter, magFilter);
    textureMap[path] = textureID;
    return textureID;
}

void TextureManager::Clear(){
    for(auto& [path, pair] : textureMap){
        TextureLoader::deleteTexture(pair.second);
    }
    textureMap.clear();
}

TextureManager::~TextureManager(){
    Clear();
}