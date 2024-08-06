#pragma once
#include "Engine/Core/Component.h"
#include "Camera.h"

class RenderableComponent : public Component
{
public:

	RenderableComponent();

	virtual void DrawShadows() = 0;
	virtual void DrawCubemapShadows()= 0;


	virtual void draw(Camera* camera) = 0;

private:

};

