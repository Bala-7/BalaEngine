#pragma once
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/MeshRenderer.h"
#include "Engine/Core/SceneGraph.h"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_widgets.h"

class Editor
{
public:
	float cubeRotation = 0;
	bool autoRotate = false;
	float rotationSpeed = 0.0f;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 directionalLightDirection;

	SceneGraph* sceneGraph;

	Editor() {}

	void Initialize();
	void Terminate();
	void DrawEditorWindows();
	void DrawPlayWindow();

	void SetDisplayedGameObject(GameObject* go);

	GameObject* GetDisplayedGameObject();

	static Editor* Instance() {
		static Editor* instance = new Editor();
		return instance;
	}

	bool IsMouseOverSceneView();

	// Delete the copy constructor and assignment operator
	Editor(const Editor&) = delete;
	void operator=(const Editor&) = delete;
protected:

private:
	GameObject* _displayedGameObject;
	GameObject* _selectedGameObject;
	bool mouseOverSceneView;
	

	void DrawInspector();

	void DrawSceneGraphWindow();
	void DrawLightingWindow();
	void DrawConsoleWindow();
	void DrawSceneViewWindow();
	void DrawObjectPickingWindow();
	void DrawShadowMapWindow();
	void DrawShadowCubeMapWindow();

	bool IsMouseOverWindow(const ImVec2& windowPos, const ImVec2& windowSize);
	void HandleMouseClickInSceneView();

	int ReadPixelID(int x, int y);
	int GetObjectIndexFromColor(glm::vec3 color);

	static int HandleInputText(ImGuiInputTextCallbackData* data);
	static std::string ImWcharToString(ImWchar imWchar);
	
	int selectedItemIndex;

	// Adding GameObjects
	bool showNewNodeTextField = false;
	bool showNewChildTextField = false;
	std::string newNodeName;
	bool showAddComponentModal = false;
};

