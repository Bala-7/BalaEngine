#include "RPShadowMapPass.h"

void RPShadowMapPass::PrepareForRender()
{
	fbo->Bind();
}

void RPShadowMapPass::Render(SceneGraph* scene, Camera* camera)
{
	scene->DrawShadows();
}

void RPShadowMapPass::Unbind()
{
	fbo->Unbind();
}

void RPShadowMapPass::CreateBuffers()
{
	fbo = new ShadowMapFBO();
	fbo->Init();
}
