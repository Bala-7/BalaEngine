#pragma once
#ifndef RENDERENGINE_H
#define RENDERENGINE_H

#define GLEW_STATIC
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Mesh.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "SpriteRenderer.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/SpriteLayer.h"
#include "Engine/Rendering/UITextRenderer.h"

class RenderEngine
{
public:
	struct Config
	{
		std::map<std::string, std::string> configValues;
		int WINDOW_SIZE_X;
		int WINDOW_SIZE_Y;
		int FPS_MAX;

		std::string FONTS_PATH;
		std::string SHADERS_PATH;

		std::string VERTEX_SHADER_PATH;
		std::string FRAGMENT_SHADER_PATH;

		std::string VERTEX_SHADER_TEXT_PATH;
		std::string FRAGMENT_SHADER_TEXT_PATH;
	};

	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

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
	void UpdateUI();
	void UpdateDebug();
	void Terminate();

	bool ShouldClose();

	GLFWwindow* GetWindow();
	Camera* GetCamera();
	GLuint GetTextureID(const char* fileName);
	GLuint GetShaderProgram();
	GLuint GetTextShaderProgram();
	std::map<char, Character> GetCharacterList();
	void AddSpriteToRenderList(SpriteRenderer* spriteRenderer);
	void AddTextToUI(UITextRenderer* textRenderer);
	void AddTextToDebugUI(UITextRenderer* textRenderer);

	Config GetConfiguration();
	int GetTargetFPS();
	int GetCurrentFPS();

	glm::vec3 GetEnvironmentLight();
	void SetEnvironmentLight(glm::vec3 value);

private:

	static RenderEngine* p_Instance;

	void InitGLFW();
	void InitGLEW();
	void InitGame();
	GLFWwindow* CreateWindow();
	
	void LoadFont(const char* path);
	void LoadConfigFile();

	void ParseValue(std::string name, std::string value);
	void InitializeConfigValues();

	Config config;
	//const int MAX_FPS = 60;
	float renderDelayMilliseconds;
	float timeSinceLastFrame;

	const int LAYER_MAX = 10;

	GLFWwindow* window;

	Camera* camera;

	// Lighting
	glm::vec3 environmentLight;

	
	const char* TEXTURES_PATH = "Assets/Textures/";
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;
	GLuint shaderProgram;
	GLuint textShaderProgram;
	std::vector<SpriteLayer*> layerList;

	std::vector<UITextRenderer*> uiRenderers;
	std::vector<UITextRenderer*> uiDebugRenderers;

	// Fonts
	std::map<char, Character> Characters;
	FT_Library ft;
	FT_Face ftFace;
};

#endif