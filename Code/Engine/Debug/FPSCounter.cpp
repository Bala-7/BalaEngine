#include "FPSCounter.h"
#include "Engine/Core/Time.h"
#include "Engine/Rendering/RenderEngine.h"

FPSCounter::FPSCounter(UITextRenderer* _text)
{
	currentTime = 0;
	text = _text;
}

void FPSCounter::Update()
{
	currentTime += Time::GetDeltaTimeMS().count();
	if (currentTime >= updateRateMS)
	{
		currentTime = 0;
		text->setText("FPS: " + std::to_string(RenderEngine::GetInstance()->GetCurrentFPS()));
		/*text->setText("FPS: " + std::to_string(testCounter));
		testCounter++;*/
	}
}

void FPSCounter::OnComponentAdded()
{

}
