#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 vp;
uniform mat4 model;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;


void main()
{
	FragPos = vec3(model * vec4(position, 1.0));
	gl_Position = vp * vec4(FragPos, 1.0);
	
	TexCoord = texCoord;	
	Normal = mat3(transpose(inverse(model))) * aNormal;
}