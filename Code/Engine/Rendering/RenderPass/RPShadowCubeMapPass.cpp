#include "RPShadowCubeMapPass.h"

void RPShadowCubeMapPass::PrepareForRender()
{
	fbo->Bind();
}

void RPShadowCubeMapPass::Render(SceneGraph* scene, Camera* camera)
{
	scene->DrawCubemapShadows();
}

void RPShadowCubeMapPass::Unbind()
{
	fbo->Unbind();

	fbo->RenderCubemapFaceToTexture();
}

void RPShadowCubeMapPass::CreateBuffers()
{
	fbo = new ShadowCubeMapFBO();
	fbo->Init();
}
