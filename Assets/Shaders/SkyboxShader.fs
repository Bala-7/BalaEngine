#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skyboxTexture;

void main()
{    
    FragColor = texture(skyboxTexture, TexCoords);
}