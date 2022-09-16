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
	newComponent->SetGameObject(this);
	newComponent->OnComponentAdded();
}

void GameObject::Update()
{
	for (auto const& component : components)
	{
		component->Update();
	}
}
