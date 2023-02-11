#version 450 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

// texture
uniform sampler2D Texture;

in vec2 TexCoord;

void main()
{
    FragColor = vec4(lightColor * objectColor, 1.0) * texture(Texture, TexCoord);;
}