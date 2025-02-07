#pragma once

class GameObject;

class Component
{
public:
	virtual void Update();
	virtual void OnComponentAdded() = 0;
	void SetGameObject(GameObject* _gameObject);
	GameObject* GetGameObject() { return gameObject; }

	enum class ComponentType
	{
		TRANSFORM,
		SPRITE_RENDERER,
		MESH_RENDERER,
		SKYBOX_RENDERER,
		UITEXT_RENDERER,
		LIGHT
	};

	ComponentType GetType();
	
	virtual void DrawInspector() = 0;


protected:
	bool isActive = true;
	GameObject* gameObject;
	ComponentType componentType;
private:
	
};

