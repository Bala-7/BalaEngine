#pragma once
#include "Engine/Rendering/SpriteRenderer.h"
#include <list>

class SpriteLayer
{
public:
	SpriteLayer();
	void Update();

	std::list<SpriteRenderer*>* GetRenderList();
private: 
	std::list<SpriteRenderer*> spritesRenderList;
};

