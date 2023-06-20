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
	void BindForWriting();
	void BindForReading(GLenum textureUnit);
	GLuint GetDepthMapTexture();
	GLuint GetDepthMapFBO();

private:
	GLuint depthMapFBO;
	GLuint depthMap;
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	glm::mat4 lightSpaceMatrix;
};

