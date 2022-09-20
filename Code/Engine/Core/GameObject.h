#pragma once
#include <list>
#include "Engine/Core/Component.h"
#include "Engine/Core/Transform.h"
#include <typeinfo>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void AddComponent(Component* newComponent);
	Component* GetComponent(Component::ComponentType type);

	void Update();

	Transform* transform;
protected:
	

private:
	std::list<Component*> components;
	
};


