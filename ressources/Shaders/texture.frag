#version 460 core
#extension GL_ARB_bindless_texture : require

out vec4 FragColor;
in vec2 TexCoords;

layout(bindless_sampler) uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(0.7f, 0.23f, 0.45f, 1.0f);
    //FragColor = vec4(TexCoords, 0.0f, 1.0f);
}