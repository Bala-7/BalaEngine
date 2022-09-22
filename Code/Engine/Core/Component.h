#pragma once

class GameObject;

class Component
{
public:
	virtual void Update();
	virtual void OnComponentAdded() = 0;
	void SetGameObject(GameObject* _gameObject);
	
	enum class ComponentType
	{
		TRANSFORM,
		SPRITE_RENDERER,
		UITEXT_RENDERER
	};

	ComponentType GetType();
	

protected:
	bool isActive = true;
	GameObject* gameObject;
	ComponentType componentType;
private:
	
};

