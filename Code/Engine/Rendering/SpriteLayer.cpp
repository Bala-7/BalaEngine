#include "SpriteLayer.h"

SpriteLayer::SpriteLayer()
{
}

void SpriteLayer::Update()
{
	for (auto const& sprite : spritesRenderList)
	{
		sprite->Update();
	}
}

std::list<SpriteRenderer*>* SpriteLayer::GetRenderList()
{
	return &spritesRenderList;
}
