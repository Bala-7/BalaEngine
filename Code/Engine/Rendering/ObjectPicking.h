#pragma once
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

class ObjectPicking
{
public:
	ObjectPicking();

	void Init();
	void RenderSceneForPicking();

private:
	GLuint pickingFBO, pickingTexture, pickingDepthBuffer;
};

