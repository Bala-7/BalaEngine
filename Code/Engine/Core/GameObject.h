#pragma once
#include <list>
#include "Engine/Core/Component.h"
#include "Engine/Core/Transform.h"
#include <typeinfo>
#include "Engine/Rendering/ModelRenderer.h"

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

	void SetRenderComponent(ModelRenderer* newComponent);
	ModelRenderer* GetRenderComponent();

	Transform* transform;
	std::string name;
protected:
	

private:
	std::list<Component*> components;
	ModelRenderer* renderComponent;
};


