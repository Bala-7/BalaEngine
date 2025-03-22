#include "RPObjectPickingPass.h"
#include <Engine/Debug/Debug.h>
#include <Engine/Rendering/RenderEngine.h>

void RPObjectPickingPass::PrepareForRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, 854, 480);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void RPObjectPickingPass::Render(SceneGraph* scene, Camera* camera)
{
	scene->DrawPicking();
}

void RPObjectPickingPass::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RPObjectPickingPass::CreateBuffers()
{
	RenderEngine::Config config = RenderEngine::GetInstance()->GetConfiguration();

	// Create a framebuffer object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);


	// Create a texture to store the color information
	glGenTextures(1, &pickingTexture);
	glBindTexture(GL_TEXTURE_2D, pickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingTexture, 0);

	// Create a renderbuffer object to store the depth information
	glGenRenderbuffers(1, &pickingDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, pickingDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickingDepthBuffer);

	// Check framebuffer completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Debug::LogError("Framebuffer is not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
