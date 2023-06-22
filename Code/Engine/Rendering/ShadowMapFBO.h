#pragma once
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

class ShadowMapFBO
{
public:
	ShadowMapFBO();

	bool Init();
	void Bind();
	void Unbind();
	GLuint GetDepthMapTexture();
	GLuint GetDepthMapFBO();

private:
	GLuint shadowMapFBO;
	GLuint shadowMapTexture;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	glm::mat4 lightSpaceMatrix;
};

