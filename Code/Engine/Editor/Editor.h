#pragma once
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/MeshRenderer.h"

class Editor
{
public:
	float cubeRotation = 0;
	bool autoRotate = false;
	float rotationSpeed = 0.0f;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	void Initialize();
	void Terminate();
	void DrawEditorWindows();

	void SetDisplayedGameObject(GameObject* go);

	GameObject* GetDisplayedGameObject();

private:
	GameObject* _displayedGameObject;

	void DrawInspector();

	void DrawComponent(Transform* transform);
	void DrawComponent(MeshRenderer* meshRenderer);
};

