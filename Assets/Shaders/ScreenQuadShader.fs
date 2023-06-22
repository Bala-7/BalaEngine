#version 450 core


out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = vec4(vec3(texture(screenTexture, TexCoords).r), 1.0f);
}