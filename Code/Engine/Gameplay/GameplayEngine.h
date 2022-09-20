#pragma once
#ifndef GAMEPLAYENGINE_H
#define GAMEPLAYENGINE_H
#include <list>
#include "Engine/Core/GameObject.h"

class GameplayEngine
{
public:
	static GameplayEngine* GetInstance() {
		if (!p_Instance)
		{
			p_Instance = new GameplayEngine();
		}
		return p_Instance;
	};


	GameplayEngine();
	~GameplayEngine();

	void Initialize();
	void Update();
	void Terminate();

	void AddGameObject(GameObject* newGO);

private:
	static GameplayEngine* p_Instance;

	std::list<GameObject*> gameObjects;
};

#endif