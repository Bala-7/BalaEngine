#pragma once
#include <list>
#include "Component.h"
#include "Transform.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void AddComponent(Component* newComponent);
	void Update();

protected:
	Transform* transform;

private:
	std::list<Component*> components;
	
};

