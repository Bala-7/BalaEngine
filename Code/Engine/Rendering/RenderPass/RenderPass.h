#pragma once
#include <GL/glew.h>
#include <Engine/Core/SceneGraph.h>

class RenderPass
{
public:
	virtual void PrepareForRender() = 0;
	virtual void Render(SceneGraph* scene, Camera* camera) = 0;
	virtual void Unbind() = 0;

	// Creates and initalizes the frame buffer and the render buffer
	virtual void CreateBuffers() = 0;

	GLuint GetTextureID() { return texture_id; }
	GLuint GetFBO() { return FBO; }

protected:
	GLuint FBO;
	GLuint RBO;
	GLuint texture_id;

};

