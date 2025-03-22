#include "RPOutlinePass.h"

void RPOutlinePass::PrepareForRender()
{
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // Render only back faces
}

void RPOutlinePass::Render(SceneGraph* scene, Camera* camera)
{
	scene->DrawOutline(camera);
}

void RPOutlinePass::Unbind()
{
	glCullFace(GL_BACK);  // Restore normal culling
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RPOutlinePass::CreateBuffers()
{
}
