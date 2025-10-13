#ifndef TEXTURE_H
#define TEXTURE_H

enum TextureType {
    DIFFUSE,
    SPECULAR
};


inline std::string TextureTypeToString(TextureType type) {
    switch (type) {
        case DIFFUSE:  return "texture_diffuse";
        case SPECULAR: return "texture_specular";
        default:       return "texture_unknown";
    }
}

struct Texture {
    unsigned int id;
    TextureType type;
    std::string path;
};

#endif