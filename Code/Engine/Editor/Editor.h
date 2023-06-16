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

	SceneGraph* sceneGraph;

	Editor() {}

	void Initialize();
	void Terminate();
	void DrawEditorWindows();

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
	bool mouseOverSceneView;

	void DrawInspector();

	void DrawSceneGraphWindow();
	void DrawLightingWindow();
	void DrawConsoleWindow();
	void DrawSceneViewWindow();

	bool IsMouseOverWindow(const ImVec2& windowPos, const ImVec2& windowSize);

	static int HandleInputText(ImGuiInputTextCallbackData* data);
	static std::string ImWcharToString(ImWchar imWchar);
	// Adding GameObjects
	bool showNewNodeTextField = false;
	std::string newNodeName;
};

