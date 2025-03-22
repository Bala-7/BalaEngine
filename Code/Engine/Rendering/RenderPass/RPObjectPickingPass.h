#pragma once
#include "RenderPass.h"
class RPObjectPickingPass : public RenderPass
{
public:

	void PrepareForRender() override;
	void Render(SceneGraph* scene, Camera* camera) override;
	void Unbind() override;
	void CreateBuffers() override;

	GLuint GetPickingTexture() { return pickingTexture; }
	GLuint GetPickingDepthBuffer() { return pickingDepthBuffer; }

private:
	GLuint pickingTexture;
	GLuint pickingDepthBuffer;
};

