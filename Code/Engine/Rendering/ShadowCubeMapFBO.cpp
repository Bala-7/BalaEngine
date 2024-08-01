#include "ShadowCubeMapFBO.h"
#include "Engine/Debug/Debug.h"
#include "RenderEngine.h"

ShadowCubeMapFBO::ShadowCubeMapFBO()
{
}

bool ShadowCubeMapFBO::Init()
{
	glGenFramebuffers(1, &shadowCubeMapFBO);
	glGenTextures(1, &shadowCubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMapTexture);
	
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
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
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Shadow cubemap tab
	glGenTextures(1, &unfoldedShadowCubeMapTexture);
	glBindTexture(GL_TEXTURE_2D, unfoldedShadowCubeMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_WIDTH * 6, SHADOW_WIDTH, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void ShadowCubeMapFBO::Bind()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowCubeMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
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
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	GLuint renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, SHADOW_WIDTH, SHADOW_WIDTH);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	for (int i = 0; i < 6; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, shadowCubeMapTexture, 0);

		// Calculate the position on the unfolded texture
		int xOffset = (i % 3) * SHADOW_WIDTH;
		int yOffset = (i / 3) * SHADOW_WIDTH;

		// Bind the unfolded texture and copy the cubemap face into it
		glBindTexture(GL_TEXTURE_2D, unfoldedShadowCubeMapTexture);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, 0, 0, SHADOW_WIDTH, SHADOW_WIDTH);
	}

	glDeleteFramebuffers(1, &framebuffer);
	glDeleteRenderbuffers(1, &renderbuffer);
}
