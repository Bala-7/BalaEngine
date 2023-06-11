// GLEW
//#define GLEW_STATIC
//#include<GL/glew.h>
// GLFW
//#include <GLFW/glfw3.h>

#include <iostream>
#include "Code/Engine/Rendering/RenderEngine.h"
#include "Engine/Rendering/MeshRenderer.h"
#include "Engine/Rendering/ModelRenderer.h"
#include "Code/Engine/Core/GameObject.h"
#include "Code/Engine/Gameplay/GameplayEngine.h"
#include "Code/Engine/Debug/Debug.h"
#include "Engine/Core/Time.h"
#include "Engine/Rendering/UITextRenderer.h"
#include "Engine/Debug/FPSCounter.h"
#include "Engine/Rendering/Model.h"
#include "Engine/Editor/Editor.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void CreateCube3D();
void CreateSprites2D();
void CreateModel();

RenderEngine* renderEngine;
GameplayEngine* gameplayEngine;
Editor* editor;

MeshRenderer* mr;
ModelRenderer* modelRenderer;
Light* light;
Model* model;

int main()
{
	

	renderEngine = new RenderEngine();
	renderEngine->Initialize();

	gameplayEngine = new GameplayEngine();
	gameplayEngine->Initialize();

	GLFWwindow* window = renderEngine->GetWindow();
	
	editor = new Editor();
	editor->Initialize();

	/*Shader ourShader("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");

	Model ourModel("resources/objects/backpack/backpack.obj");*/

	//CreateCube3D();
	//CreateSprites2D();
	CreateModel();

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
			
			if(editor->autoRotate)
			{
				editor->GetDisplayedGameObject()->transform->rotation = glm::vec3(0.0f, editor->cubeRotation, 0.0f);
				editor->cubeRotation += editor->rotationSpeed * dt;
			}
			accumulator -= dt;
			t += dt;
		}
		
		// Scene Rendering code
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);//clear yellow
		//mr->draw();
		//model->Draw();
		modelRenderer->draw();

		editor->DrawEditorWindows();

		//renderEngine->Update();
		// \Scene Rendering code\
			
		// GUI Rendering code
		//renderEngine->UpdateUI();
		//renderEngine->UpdateDebug();
		// \GUI Rendering code
		renderEngine->SetEnvironmentLight(glm::vec3(editor->color[0], editor->color[1], editor->color[2]));
		glfwSwapBuffers(window);
	}

	editor->Terminate();

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

void CreateCube3D()
{
	GameObject* lightGO = new GameObject();
	light = new Light(Light::LightType::Point,
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
	
	GameObject* meshGO = new GameObject();
	mr = new MeshRenderer(MeshType::kCube);
	mr->setTexture(RenderEngine::GetInstance()->GetTextureID("Concrete.jpg"));
	mr->setProgram(RenderEngine::GetInstance()->GetShaderProgram());
	mr->shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mr->shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	meshGO->transform->position = glm::vec3(0.0f, 0.0f, -30.0f);
	meshGO->transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	meshGO->transform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	meshGO->AddComponent(mr);

	editor->SetDisplayedGameObject(meshGO);
}

void CreateSprites2D()
{
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
}

void CreateModel()
{
	GameObject* lightGO = new GameObject();
	light = new Light(Light::LightType::Point,
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));

	GameObject* meshGO = new GameObject();
	modelRenderer = new ModelRenderer("Assets/Models/Samus/Samus/Samus_small.obj");

	/*mr = new MeshRenderer(MeshType::kModel);
	mr->setTexture(RenderEngine::GetInstance()->GetTextureID("Concrete.jpg"));
	mr->setProgram(RenderEngine::GetInstance()->GetShaderProgram());
	mr->shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mr->shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));*/

	meshGO->transform->position = glm::vec3(0.05f, -2.0f, 5.0f);
	meshGO->transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	meshGO->transform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	meshGO->AddComponent(modelRenderer);

	editor->SetDisplayedGameObject(meshGO);

	//model = new Model("Assets/Models/example.obj");
}