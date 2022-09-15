#include "GameObject.h"

GameObject::GameObject()
{
	transform = new Transform();
	AddComponent(transform);
}

GameObject::~GameObject()
{
}

void GameObject::AddComponent(Component* newComponent)
{
	components.push_back(newComponent);
}

void GameObject::Update()
{
	for (auto const& component : components)
	{
		component->Update();
	}
}
