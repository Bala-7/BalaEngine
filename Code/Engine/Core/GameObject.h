#pragma once
#include <list>
#include "Engine/Core/Component.h"
#include "Engine/Core/Transform.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void AddComponent(Component* newComponent);
	void Update();

	Transform* transform;
protected:
	

private:
	std::list<Component*> components;
	
};

