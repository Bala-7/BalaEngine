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
#include "Engine/Core/SceneGraph.h"
#include "Engine/Rendering/ShadowMapFBO.h"
#include "Engine/Rendering/ShadowCubeMapFBO.h"

class RenderEngine
{
public:
	struct Config
	{
		std::map<std::string, std::string> configValues;
		int WINDOW_SIZE_X;
		int WINDOW_SIZE_Y;
		int FPS_MAX;

		int SCENE_VIEW_SIZE_X;
		int SCENE_VIEW_SIZE_Y;

		int PLAY_VIEW_SIZE_X;
		int PLAY_VIEW_SIZE_Y;

		std::string FONTS_PATH;
		std::string SHADERS_PATH;

		std::string VERTEX_SHADER_PATH;
		std::string FRAGMENT_SHADER_PATH;		
		
		std::string VERTEX_SHADER_PATH_OUTLINE;
		std::string FRAGMENT_SHADER_PATH_OUTLINE;

		std::string VERTEX_SHADER_PATH_SKYBOX;
		std::string FRAGMENT_SHADER_PATH_SKYBOX;

		std::string VERTEX_SHADER_TEXT_PATH;
		std::string FRAGMENT_SHADER_TEXT_PATH;

		std::string VERTEX_SHADER_SHADOW_PATH;
		std::string FRAGMENT_SHADER_SHADOW_PATH;

		std::string VERTEX_SHADER_PICKING_PATH;
		std::string FRAGMENT_SHADER_PICKING_PATH;

		std::string VERTEX_SHADER_CM_SHADOW_PATH;
		std::string FRAGMENT_SHADER_CM_SHADOW_PATH;
		std::string GEOMETRY_SHADER_CM_SHADOW_PATH;
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
	GLFWwindow* GetPlayWindow();
	void UseWindow(GLFWwindow* window);
	Camera* GetCamera();
	GLuint GetTextureID(const char* fileName, bool fullPath = false);
	GLuint GetShaderProgram();
	GLuint GetSkyboxShaderProgram();
	GLuint GetShadowShaderProgram();
	GLuint GetPickingShaderProgram();
	GLuint GetCubeMapShadowShaderProgram();
	GLuint GetTextShaderProgram();
	std::map<char, Character> GetCharacterList();
	void AddSpriteToRenderList(SpriteRenderer* spriteRenderer);
	void AddTextToUI(UITextRenderer* textRenderer);
	void AddTextToDebugUI(UITextRenderer* textRenderer);

	Config GetConfiguration();
	int GetTargetFPS();
	int GetCurrentFPS();

	// Lighting
	glm::vec3 GetEnvironmentLight();
	glm::vec3 GetDirectionalLightDirection();
	void SetEnvironmentLight(glm::vec3 value);
	void SetDirectionalLightDirection(glm::vec3 value);

	void CreateFramebuffer();
	void CreateShadowmapFramebuffer();
	void CreateObjectPickingFramebuffer();
	void BindFramebuffer(GLuint fbo);
	void UnbindFramebuffer();
	void RescaleFramebuffer(float width, float height);

	GLuint GetFrameBufferTexture() { return texture_id; }
	GLuint GetPlayWindowFrameBufferTexture() { return playWindowTexture; }
	GLuint GetObjectPickingTexture() { return pickingTexture; }
	
	void OnKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mode);
	void OnMouseInput(GLFWwindow* window, int button, int action, int mods);
	void OnCursorPositionInput(GLFWwindow* window, double xpos, double ypos);

	void RenderSceneView(SceneGraph* scene);
	void RenderPlayView(SceneGraph* scene);

	// Shadows
	GLuint GetDepthMapTexture() { return shadowMap->GetDepthMapTexture(); }
	GLuint GetDepthMapFBO() { return shadowMap->GetDepthMapFBO(); }
	GLuint GetDepthCubeMapTexture() { return shadowCubeMap->GetDepthMapTexture(); }
	GLuint GetDepthCubeMapTextureUnfolded() { return shadowCubeMap->GetDepthMapTextureUnfolded(); }
	GLuint GetDepthCubeMapFBO() { return shadowCubeMap->GetDepthMapFBO(); }
	glm::mat4 GetLightProjectionMatrix() { return lightProjectionMatrix; }
	glm::mat4 GetLightViewMatrix() { return lightViewMatrix; }
	glm::mat4 GetLightViewProjectionMatrix() { return lightViewProjectionMatrix; }
	void CalculateLightViewMatrices();

	void SetLightViewProjectionMatrix(glm::mat4 newMatrix) { lightViewProjectionMatrix = newMatrix; }

	// Object picking
	GLuint GetOutlineShaderProgram();
	GLuint GetPickingTexture() { return pickingTexture; }
	GLuint GetPickingFBO() { return pickingFBO; }
	void StartObjectPicking() { renderedItems.clear(); objectIndex = 1; }
	void IncrementObjectPickingIndex() 
	{ 
		objectIndex++; 
	}
	int GetCurrentObjectPickingIndex() 
	{ 
		return objectIndex; 
	}
	void AddToRenderedItems(GameObject* go) 
	{ 
		renderedItems.push_back(go); 
	}
	GameObject* GetRenderedItem(int index) 
	{ 
		return renderedItems[index]; 
	}

	// Play Mode
	void StartPlayMode();

private:

	static RenderEngine* p_Instance;

	void InitGLFW();
	void InitGLEW();
	void InitGame();
	GLFWwindow* CreateWindow();
	GLFWwindow* CreatePlayWindow();

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
	GLFWwindow* playWindow;

	Camera* sceneViewCamera;
	Camera* playViewCamera;

	// Lighting
	glm::vec3 environmentLight;
	glm::vec3 directionalLightDirection;
	float directionalLightIntensity;
	
	const char* TEXTURES_PATH = "Assets/Textures/";
	TextureLoader textureLoader;
	ShaderLoader shaderLoader;
	GLuint shaderProgram;
	GLuint skyboxShaderProgram;
	GLuint textShaderProgram;
	std::vector<SpriteLayer*> layerList;

	std::vector<UITextRenderer*> uiRenderers;
	std::vector<UITextRenderer*> uiDebugRenderers;

	// Fonts
	std::map<char, Character> Characters;
	FT_Library ft;
	FT_Face ftFace;

	// Framebuffer display for Scene window
	GLuint FBO;
	GLuint RBO;
	GLuint texture_id;

	// Framebuffer display for Play window
	GLuint playWindowFBO;
	GLuint playWindowRBO;
	GLuint playWindowTexture;

	// Input
	bool isShiftKeyPressed = false;
	bool isMouseCenterButtonPressed = false;

	float lastMouseX = 0.0f;
	float lastMouseY = 0.0f;


	// Shadows
	ShadowMapFBO* shadowMap;
	GLuint shadowShaderProgram;
	ShadowCubeMapFBO* shadowCubeMap;
	GLuint shadowCubeMapShaderProgram;

	// Object picking in Scene View
	GLuint pickingFBO, pickingTexture, pickingDepthBuffer, pickingShaderProgram, outlineShaderProgram;
	int objectIndex;
	std::vector<GameObject*> renderedItems;


	glm::mat4 lightProjectionMatrix;
	glm::mat4 lightViewMatrix;
	glm::mat4 lightViewProjectionMatrix;
};

#endif