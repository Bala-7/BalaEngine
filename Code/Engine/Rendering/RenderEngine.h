#pragma once
#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#define GLEW_STATIC
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <list>
#include "Mesh.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "SpriteRenderer.h"
#include "Engine/Core/GameObject.h"


class RenderEngine
{
public:
	static RenderEngine* GetInstance() { 
		if (!p_Instance)
		{
			p_Instance = new RenderEngine();
		}
		return p_Instance; 
	};

	RenderEngine();
	~RenderEngine();

	void Initialize();
	void Update();
	void Terminate();

	bool ShouldClose();
	GLFWwindow* GetWindow();
	Camera* GetCamera();
	GLuint GetTextureID(const char* fileName);
	GLuint GetShaderProgram();
	void AddSpriteToRenderList(SpriteRenderer* spriteRenderer);

private:
	static RenderEngine* p_Instance;

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
	//MeshRenderer* mesh;
	/*SpriteRenderer* sprite;
	SpriteRenderer* sprite2;
	GameObject* go;
	GameObject* go2;*/
	
	const char* TEXTURES_PATH = "Assets/Textures/";
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;
	GLuint shaderProgram;
	std::list<SpriteRenderer*> spritesRenderList;

};

#endif