#pragma once
#include <list>
#include "Engine/Core/Component.h"
#include "Engine/Core/Transform.h"
#include <typeinfo>
#include "Engine/Rendering/ModelRenderer.h"
#include "Engine/Rendering/RenderableComponent.h"

class SceneNode;

class GameObject
{
public:
	GameObject();
	GameObject(std::string _name);
	~GameObject();

	void AddComponent(Component* newComponent);
	Component* GetComponent(Component::ComponentType type);
	std::list<Component*> GetComponentsList();

	void Update();

	void DrawInspector();

	void SetRenderComponent(RenderableComponent* newComponent);
	RenderableComponent* GetRenderComponent();

	void SetSceneNode(SceneNode* newNode);
	SceneNode* GetNode() { return node; }

	Transform* transform;
	std::string name;

	void OnEditorSelect();
	void OnEditorUnSelect();
	bool IsSelectedInEditor() { return isSelectedInEditor; };

protected:
	

private:
	bool isSelectedInEditor = false;
	std::list<Component*> components;
	RenderableComponent* renderComponent;
	SceneNode* node;
};


