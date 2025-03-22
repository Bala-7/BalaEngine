#pragma once
#include <Engine/Rendering/RenderPass/RenderPass.h>
#include <Engine/Core/SceneGraph.h>

class RPLightPass : public RenderPass
{
public:
	RPLightPass();

	void PrepareForRender() override;
	void Render(SceneGraph* scene, Camera* camera) override;
	void Unbind() override;
	void CreateBuffers() override;

	void SetViewSize(int x, int y);

private:
	int sceneSizeX, sceneSizeY;
};

