#include "GameObject.h"
#include "Engine/Gameplay/GameplayEngine.h"

GameObject::GameObject()
{
	transform = new Transform();
	AddComponent(transform);
	GameplayEngine::GetInstance()->AddGameObject(this);
}

GameObject::GameObject(std::string _name)
{
	name = _name;
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

void GameObject::DrawInspector()
{
	for (auto i : components)
		i->DrawInspector();
}

void GameObject::SetRenderComponent(RenderableComponent* newComponent)
{
	renderComponent = newComponent;
}

RenderableComponent* GameObject::GetRenderComponent()
{
	return renderComponent;
}

void GameObject::SetSceneNode(SceneNode* newNode)
{
	node = newNode;
}

void GameObject::OnEditorSelect()
{
	isSelectedInEditor = true;
}

void GameObject::OnEditorUnSelect()
{
	isSelectedInEditor = false;
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

std::list<Component*> GameObject::GetComponentsList()
{
	return components;
}

