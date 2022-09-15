#pragma once
#define GLEW_STATIC
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "ShaderLoader.h"
#include "Camera.h"
#include "LightRenderer.h"
#include "TextureLoader.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"

class RenderEngine
{
public:
	RenderEngine();
	~RenderEngine();

	void Initialize();
	void Update();
	void Terminate();

	bool ShouldClose();
	GLFWwindow* GetWindow();

private:
	void InitGLFW();
	void InitGLEW();
	void InitGame();
	GLFWwindow* CreateWindow();
	
	const int WINDOW_SIZE_X = 1280;
	const int WINDOW_SIZE_Y = 720;

	const char* VERTEX_SHADER_PATH = "Assets/Shaders/TexturedModel.vs";
	const char* FRAGMENT_SHADER_PATH = "Assets/Shaders/TexturedModel.fs";

	GLFWwindow* window;

	Camera* camera;
	LightRenderer* light;
	MeshRenderer* mesh;
	SpriteRenderer* sprite;
	SpriteRenderer* sprite2;
};

