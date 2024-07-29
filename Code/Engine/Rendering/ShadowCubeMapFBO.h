#pragma once
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class ShadowCubeMapFBO
{
public:
	ShadowCubeMapFBO();

	bool Init();
	void Bind();
	void Unbind();
	GLuint GetDepthMapTexture();
	GLuint GetDepthMapFBO();

private:
	GLuint shadowCubeMapFBO;
	GLuint shadowCubeMapTexture;
	const GLuint SHADOW_WIDTH = 128, SHADOW_HEIGHT = 128;

	glm::mat4 lightSpaceMatrix;
};

