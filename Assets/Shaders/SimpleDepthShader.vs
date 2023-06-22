#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 lightvp;
uniform mat4 modelMatrix;

//out vec4 fragPosition;

void main()
{
	gl_Position = lightvp * modelMatrix * vec4(position, 1.0f);
	//fragPosition = gl_Position;
}