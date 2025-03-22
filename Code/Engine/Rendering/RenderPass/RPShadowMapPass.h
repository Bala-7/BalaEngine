#pragma once
#include "RenderPass.h"
#include <Engine/Rendering/ShadowMapFBO.h>

class RPShadowMapPass : public RenderPass
{
public:
	void PrepareForRender() override;
	void Render(SceneGraph* scene, Camera* camera) override;
	void Unbind() override;
	void CreateBuffers() override;

	ShadowMapFBO* GetShadowMapFBO() { return fbo; }
private:
	ShadowMapFBO* fbo;
};

