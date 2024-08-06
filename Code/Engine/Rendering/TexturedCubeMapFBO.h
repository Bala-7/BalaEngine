#pragma once
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <vector>
#include <string>

class TexturedCubeMapFBO
{
public:
	TexturedCubeMapFBO();

	unsigned int LoadCubemap(std::vector<std::string> faces);
private:
	GLuint fbo;
	GLuint texture;
};

