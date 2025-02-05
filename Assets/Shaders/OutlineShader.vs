#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoord;

uniform mat4 vp;
uniform mat4 model;
uniform vec3 cameraPos; // Camera world position

void main()
{
	// Get object position in world space
    vec3 worldPos = vec3(model * vec4(position, 1.0));
	
	// Compute distance from camera to object
    float distanceToCamera = length(cameraPos - worldPos);

	// Scale factor based on distance (adjust 0.002 to tweak effect)
    float outlineScale = 1.0 + (distanceToCamera * 0.002); 

	// Apply the dynamic scale
    mat4 scaledModel = model * mat4(outlineScale);

	vec4 pos = vp * vec4(vec3(scaledModel * vec4(position, 1.0)), 1.0);
	//pos.z -= 0.01 * pos.w;
	gl_Position = pos;
		
	
}