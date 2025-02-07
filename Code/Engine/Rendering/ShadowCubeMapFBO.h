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
	GLuint GetDepthMapTextureUnfolded();
	GLuint GetDepthMapFBO();

	void RenderCubemapFaceToTexture();

private:
	GLuint shadowCubeMapFBO;
	GLuint shadowCubeMapTexture;
	GLuint depthBuffer;
	GLuint unfoldedShadowCubeMapTexture;
	const GLuint SHADOW_SIZE = 1024;


	// Render the cubemap to a texture
	GLuint framebufferRenderToTexture;
	GLuint renderbufferRenderToTexture;

	glm::mat4 lightSpaceMatrix;
};

