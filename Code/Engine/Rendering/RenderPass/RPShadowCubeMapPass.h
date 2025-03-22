#pragma once
#include <Engine/Rendering/RenderPass/RenderPass.h>
#include <Engine/Core/SceneGraph.h>
#include <Engine/Rendering/ShadowCubeMapFBO.h>

class RPShadowCubeMapPass : public RenderPass
{
public:

	void PrepareForRender() override;
	void Render(SceneGraph* scene, Camera* camera) override;
	void Unbind() override;
	void CreateBuffers() override;

	ShadowCubeMapFBO* GetCubeMapFBO() { return fbo; }
private:
	ShadowCubeMapFBO* fbo;
};

