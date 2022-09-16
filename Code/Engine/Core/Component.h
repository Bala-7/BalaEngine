#pragma once

class GameObject;

class Component
{
public:
	virtual void Update();
	virtual void OnComponentAdded() = 0;
	void SetGameObject(GameObject* _gameObject);
	
protected:
	bool isActive = true;
	GameObject* gameObject;
private:
	
};

