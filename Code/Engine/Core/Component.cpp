#include "Component.h"

void Component::Update()
{
}

void Component::SetGameObject(GameObject* _gameObject)
{
	gameObject = _gameObject;
}

Component::ComponentType Component::GetType()
{
	return componentType;
}



