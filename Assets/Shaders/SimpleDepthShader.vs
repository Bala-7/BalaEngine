#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 lightvp;
uniform mat4 modelMatrix;

void main()
{
	vec4 worldPos = modelMatrix * vec4(position, 1.0f);
	gl_Position = lightvp * worldPos;
}