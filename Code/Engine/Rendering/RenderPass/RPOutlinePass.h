#pragma once
#include "RenderPass.h"
class RPOutlinePass : public RenderPass
{
public:
	void PrepareForRender() override;
	void Render(SceneGraph* scene, Camera* camera) override;
	void Unbind() override;
	void CreateBuffers() override;
};

