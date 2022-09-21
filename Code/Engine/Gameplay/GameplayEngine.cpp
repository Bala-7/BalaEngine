#include "GameplayEngine.h"
#include "Engine/Debug/Debug.h"

GameplayEngine* GameplayEngine::p_Instance;

GameplayEngine::GameplayEngine()
{
	p_Instance = this;
}

GameplayEngine::~GameplayEngine()
{
}

void GameplayEngine::Initialize()
{
	Debug::Log("Gameplay Engine initialized!");
}

void GameplayEngine::Update()
{
	for (auto const& gameObject : gameObjects)
	{
		gameObject->Update();
	}
}

void GameplayEngine::Terminate()
{

}

void GameplayEngine::AddGameObject(GameObject* newGO)
{
	gameObjects.push_back(newGO);
}
