#include "ShadowMapFBO.h"
#include "Engine/Debug/Debug.h"
#include "RenderEngine.h"

ShadowMapFBO::ShadowMapFBO()
{
}

bool ShadowMapFBO::Init()
{
	glGenTextures(1, &shadowMapTexture);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) 
	{
		Debug::LogError("Shadowmap framebuffer error");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	Debug::Log("Shadowmap framebuffer initialized!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return true;
}

void ShadowMapFBO::Bind()
{
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMapFBO::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderEngine::GetInstance()->SetDepthMapTexture(shadowMapTexture);
}

GLuint ShadowMapFBO::GetDepthMapTexture()
{
	return shadowMapTexture;
}

GLuint ShadowMapFBO::GetDepthMapFBO()
{
	return shadowMapFBO;
}