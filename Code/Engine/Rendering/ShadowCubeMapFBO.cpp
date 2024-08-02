#include "ShadowCubeMapFBO.h"
#include "Engine/Debug/Debug.h"
#include "RenderEngine.h"

ShadowCubeMapFBO::ShadowCubeMapFBO()
{
}

bool ShadowCubeMapFBO::Init()
{
	 // New method
	/*// Create the depth buffer
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the cube map
	glGenTextures(1, &shadowCubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for (GLuint i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, SHADOW_SIZE, SHADOW_SIZE, 0, GL_RED, GL_FLOAT, NULL);
	}

	// Create the FBO
	glGenFramebuffers(1, &shadowCubeMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowCubeMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	// Disable write and reads to and from the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	*/

	// Previous version from here to bottom
	glGenFramebuffers(1, &shadowCubeMapFBO);
	glGenTextures(1, &shadowCubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMapTexture);
	
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		Debug::LogError("Shadowcubemap framebuffer error");
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, shadowCubeMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubeMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	Debug::Log("Shadowcubemap framebuffer initialized!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// unfolded texture for editor tab
	glGenTextures(1, &unfoldedShadowCubeMapTexture);
	glBindTexture(GL_TEXTURE_2D, unfoldedShadowCubeMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_SIZE * 6, SHADOW_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenFramebuffers(1, &framebufferRenderToTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferRenderToTexture);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, unfoldedShadowCubeMapTexture, 0);

	glGenRenderbuffers(1, &renderbufferRenderToTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferRenderToTexture);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, SHADOW_SIZE, SHADOW_SIZE);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbufferRenderToTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return true;
}

void ShadowCubeMapFBO::Bind()
{
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowCubeMapFBO);
	glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	glCullFace(GL_FRONT);
}

void ShadowCubeMapFBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

GLuint ShadowCubeMapFBO::GetDepthMapTexture()
{
	return shadowCubeMapTexture;
}

GLuint ShadowCubeMapFBO::GetDepthMapTextureUnfolded()
{
	return unfoldedShadowCubeMapTexture;
}

GLuint ShadowCubeMapFBO::GetDepthMapFBO()
{
	return shadowCubeMapFBO;
}

void ShadowCubeMapFBO::RenderCubemapFaceToTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferRenderToTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferRenderToTexture);
	
	struct FaceCoords {
		GLint x, y;
	};

	FaceCoords faceCoords[6] = {
	{0, SHADOW_SIZE},            // +X
	{SHADOW_SIZE, SHADOW_SIZE},     // -X
	{2 * SHADOW_SIZE, SHADOW_SIZE}, // +Y
	{0, 0},                   // -Y
	{SHADOW_SIZE, 0},            // +Z
	{2 * SHADOW_SIZE, 0}         // -Z
	};
	/*
	for (GLuint face = 0; face < 6; ++face) {
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, shadowCubeMapTexture, 0);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE) {
			std::cerr << "Framebuffer not complete!" << std::endl;
			return;
		}

		glBindTexture(GL_TEXTURE_2D, unfoldedShadowCubeMapTexture);

		FaceCoords coords = faceCoords[face];
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, coords.x, coords.y, 0, 0, SHADOW_WIDTH, SHADOW_WIDTH);
	}*/

	// Take only one face
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, shadowCubeMapTexture, 0);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer not complete!" << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D, unfoldedShadowCubeMapTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, SHADOW_SIZE, SHADOW_SIZE);
	


	/* //Uncomment this to draw red color into the displayed texture
	// Define the color to fill the texture with (e.g., red color)
	const unsigned char color[3] = { 255, 0, 0 }; // RGB for red

	// Create an array to fill the texture
	unsigned char* data = new unsigned char[SHADOW_WIDTH * SHADOW_WIDTH * 3]; // 3 bytes per pixel (RGB)

	// Fill the array with the color data
	for (int i = 0; i < SHADOW_WIDTH * SHADOW_WIDTH; ++i) {
		data[i * 3 + 0] = color[0]; // Red
		data[i * 3 + 1] = color[1]; // Green
		data[i * 3 + 2] = color[2]; // Blue
	}

	// Update the texture
	glBindTexture(GL_TEXTURE_2D, unfoldedShadowCubeMapTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SHADOW_WIDTH, SHADOW_WIDTH, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	*/

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	//glDeleteFramebuffers(1, &framebuffer);
	//glDeleteRenderbuffers(1, &renderbuffer);
}
