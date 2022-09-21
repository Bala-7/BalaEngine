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
	sr->setLayer(1);
	go->AddComponent(sr);
	go->transform->position = glm::vec3(-0.5f, 0.0f, 0.0f);

	GameObject* go2 = new GameObject();
	SpriteRenderer* sr2 = new SpriteRenderer();
	sr2->setSpriteSheet("Character.png", 32, 32, 3, 4);
	sr2->setSprite(10);
	sr2->setLayer(0);
	go2->AddComponent(sr2);
	go2->transform->position = glm::vec3(0.5f, 0.0f, 0.0f);



	glfwSetKeyCallback(window, key_callback);	// Input callback
	// RendererInitialization();	// Triangle renderer
	while (!renderEngine->ShouldClose())
	{
		glfwPollEvents();

		// Physics code
		// TODO
		// \Physics code\

		// Game Logic code
		gameplayEngine->Update();
		// \Game Logic code\

		Time::StartFrame();
		// Scene Rendering code
		renderEngine->Update();
		// \Scene Rendering code\
		
		Time::EndFrame();
		//Debug::Log(std::to_string(Time::GetDeltaTime().count()));

		// GUI Rendering code
		// TODO
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