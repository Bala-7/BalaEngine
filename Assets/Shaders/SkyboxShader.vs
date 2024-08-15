#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 vp;

void main()
{
    TexCoords = vec3(aPos.x, -aPos.y, aPos.z);
	vec4 pos = vp * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  