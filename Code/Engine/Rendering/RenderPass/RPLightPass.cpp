#include "RPLightPass.h"
#include <Engine/Rendering/RenderEngine.h>
RPLightPass::RPLightPass()
{
}

void RPLightPass::PrepareForRender()
{
	RenderEngine::Config config = RenderEngine::GetInstance()->GetConfiguration();

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
}

void RPLightPass::Render(SceneGraph* scene, Camera* camera)
{
	scene->Draw(camera);
}

void RPLightPass::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RPLightPass::CreateBuffers()
{
	RenderEngine::Config config = RenderEngine::GetInstance()->GetConfiguration();

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sceneSizeX, sceneSizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, sceneSizeX, sceneSizeY);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	GLint stencil;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &stencil);
	printf("Stencil Attachment: %d\n", stencil);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || stencil != GL_RENDERBUFFER)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

}

void RPLightPass::SetViewSize(int x, int y)
{
	sceneSizeX = x;
	sceneSizeY = y;
}
