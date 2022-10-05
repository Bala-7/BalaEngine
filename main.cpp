// GLEW
//#define GLEW_STATIC
//#include<GL/glew.h>
// GLFW
//#include <GLFW/glfw3.h>

#include <iostream>
#include "Code/Engine/Rendering/RenderEngine.h"
#include "Code/Engine/Core/GameObject.h"
#include "Code/Engine/Gameplay/GameplayEngine.h"
#include "Code/Engine/Debug/Debug.h"
#include "Engine/Core/Time.h"
#include "Engine/Rendering/UITextRenderer.h"
#include "Engine/Debug/FPSCounter.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

RenderEngine* renderEngine;
GameplayEngine* gameplayEngine;

int main()
{
	renderEngine = new RenderEngine();
	renderEngine->Initialize();

	gameplayEngine = new GameplayEngine();
	gameplayEngine->Initialize();

	GLFWwindow* window = renderEngine->GetWindow();
	
	GameObject* go = new GameObject();
	SpriteRenderer* sr = new SpriteRenderer();
	sr->setSpriteSheet("PokemonBlue_Player_Spritesheet.png", 16, 16, 10, 1);
	sr->setSprite(1);
	sr->setLayer(0);
	go->transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
	go->AddComponent(sr);
	

	GameObject* go2 = new GameObject();
	SpriteRenderer* sr2 = new SpriteRenderer();
	sr2->setSpriteSheet("Character.png", 32, 32, 3, 4);
	sr2->setSprite(10);
	sr2->setLayer(1);
	go2->transform->position = glm::vec3(0.5f, 0.0f, 0.0f);
	go2->AddComponent(sr2);
	


	GameObject* textGO = new GameObject();
	UITextRenderer* textRenderer = new UITextRenderer();
	textRenderer->setText("This is sample text");
	textRenderer->setPosition(glm::vec2(450.0f, 240.0f));
	textRenderer->setScale(0.85f);
	textRenderer->setColor(glm::vec3(0.3f, 0.7f, 0.9f));
	textGO->AddComponent(textRenderer);

	GameObject* fpsTextGO = new GameObject();
	UITextRenderer* fpsTR = new UITextRenderer();
	fpsTR->setText("FPS: ");
	fpsTR->setPosition(glm::vec2(5.0f, 695.0f));
	fpsTR->setScale(0.45f);
	fpsTR->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	fpsTR->setDebug(true);
	fpsTextGO->AddComponent(fpsTR);
	FPSCounter* counter = new FPSCounter(fpsTR);
	fpsTextGO->AddComponent(counter);


	glfwSetKeyCallback(window, key_callback);	// Input callback
	// RendererInitialization();	// Triangle renderer

	auto currentTime = Time::Now();
	float t = 0.0f;
	const float dt = 1.0f / renderEngine->GetTargetFPS();
	float accumulator = 0.0f;

	while (!renderEngine->ShouldClose())
	{
		auto newTime = Time::Now();
		float frameTime = (newTime.count() - currentTime.count()) / 1000000.0f;
		Time::SetLastFrameTime(frameTime);
		currentTime = newTime;
		
		accumulator += frameTime;

		glfwPollEvents();

		while (accumulator >= dt) 
		{
			// Physics code
			// TODO
			// \Physics code\

			// Game Logic code
			gameplayEngine->Update();
			// \Game Logic code\

			accumulator -= dt;
			t += dt;
		}
		
		// Scene Rendering code
		renderEngine->Update();
		// \Scene Rendering code\
			
		// GUI Rendering code
		renderEngine->UpdateUI();
		renderEngine->UpdateDebug();
		// \GUI Rendering code

		glfwSwapBuffers(window);
	}


	renderEngine->Terminate();

	return 0;
}


// -----------------------------------
// -- Input --------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// -----------------------------------