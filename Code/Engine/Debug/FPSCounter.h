#pragma once
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/UITextRenderer.h"
#include <chrono>

class FPSCounter : public Component
{
public:
	FPSCounter(UITextRenderer* _text);

	void Update() override;
	void OnComponentAdded() override;
	void DrawInspector() override;

private:
	float currentTime;
	int updateRateMS = 1000;

	UITextRenderer* text;

	int testCounter = 0;
};
