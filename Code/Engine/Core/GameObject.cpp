#include "GameObject.h"
#include "Engine/Gameplay/GameplayEngine.h"

GameObject::GameObject()
{
	transform = new Transform();
	AddComponent(transform);
	GameplayEngine::GetInstance()->AddGameObject(this);
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
		if(component->GetType() != Component::ComponentType::SPRITE_RENDERER)
			component->Update();
	}
}

Component* GameObject::GetComponent(Component::ComponentType type)
{
	for (auto const& component : components)
	{
		if (component->GetType() == type)
			return component;
	}

	return NULL;
}

