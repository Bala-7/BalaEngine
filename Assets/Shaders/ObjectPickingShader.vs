#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 vp;
uniform mat4 model;

out vec3 FragPos;
out float interpolatedDepth;

void main()
{
	FragPos = vec3(model * vec4(position, 1.0));
	gl_Position = vp * model * vec4(position, 1.0);
	
	
	interpolatedDepth = gl_Position.z / gl_Position.w;
	
}