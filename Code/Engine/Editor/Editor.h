#pragma once
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/MeshRenderer.h"
#include "Engine/Core/SceneGraph.h"

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

	// Delete the copy constructor and assignment operator
	Editor(const Editor&) = delete;
	void operator=(const Editor&) = delete;
protected:

private:
	GameObject* _displayedGameObject;

	void DrawInspector();

	void DrawSceneGraphWindow();
	void DrawLightingWindow();
	void DrawSceneViewWindow();
};
