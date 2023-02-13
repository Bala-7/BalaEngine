// GLEW
//#define GLEW_STATIC
//#include<GL/glew.h>
// GLFW
//#include <GLFW/glfw3.h>

#include <iostream>
#include "Code/Engine/Rendering/RenderEngine.h"
#include "Engine/Rendering/MeshRenderer.h"
#include "Code/Engine/Core/GameObject.h"
#include "Code/Engine/Gameplay/GameplayEngine.h"
#include "Code/Engine/Debug/Debug.h"
#include "Engine/Core/Time.h"
#include "Engine/Rendering/UITextRenderer.h"
#include "Engine/Debug/FPSCounter.h"
#include "Engine/Rendering/Model.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_widgets.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void CreateCube3D();
void CreateSprites2D();

RenderEngine* renderEngine;
GameplayEngine* gameplayEngine;

int main()
{
	renderEngine = new RenderEngine();
	renderEngine->Initialize();

	gameplayEngine = new GameplayEngine();
	gameplayEngine->Initialize();

	GLFWwindow* window = renderEngine->GetWindow();
	
	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
	// \IMGUI

	/*Shader ourShader("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");

	Model ourModel("resources/objects/backpack/backpack.obj");*/

	//CreateCube3D();
	CreateSprites2D();
	GameObject* go = new GameObject();
	MeshRenderer* mr = new MeshRenderer(MeshType::kCube);
	mr->setTexture(RenderEngine::GetInstance()->GetTextureID("Concrete.jpg"));
	mr->setProgram(RenderEngine::GetInstance()->GetShaderProgram());
	mr->shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	mr->shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	go->transform->position = glm::vec3(0.0f, 0.5f, 4.0f);
	go->transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	go->transform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	go->AddComponent(mr);

	glfwSetKeyCallback(window, key_callback);	// Input callback
	// RendererInitialization();	// Triangle renderer

	auto currentTime = Time::Now();
	float t = 0.0f;
	const float dt = 1.0f / renderEngine->GetTargetFPS();
	float accumulator = 0.0f;

	float rotation = 0;
	bool drawCube = true;
	float rotationSpeed = 0.0f;
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec3 position = glm::vec3(0.0f);

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
			
			go->transform->position = position;
			go->transform->rotation = glm::vec3(0.0f, rotation, 0.0f);
			rotation += rotationSpeed * dt;

			accumulator -= dt;
			t += dt;
		}
		
		// Scene Rendering code
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0);//clear yellow
		if(drawCube)
			mr->draw();
	

		// ImGui render
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("ImGui Editor Window");
		ImGui::Text("Edit object parameters");
		ImGui::Checkbox("Draw Cube", &drawCube);
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.0f, 20.0f);
		ImGui::ColorEdit4("Light Color", color);
		if (ImGui::CollapsingHeader("Cube"))
		{
			ImGui::Separator();
			ImGui::Text("Position");
			ImGui::Separator();
			nimgui::draw_vec3_widget("Position", position);
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// \ImGui render

		//renderEngine->Update();
		// \Scene Rendering code\
			
		// GUI Rendering code
		//renderEngine->UpdateUI();
		//renderEngine->UpdateDebug();
		// \GUI Rendering code
		renderEngine->SetEnvironmentLight(glm::vec3(color[0], color[1], color[2]));
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
	GameObject* go = new GameObject();
	MeshRenderer* mr = new MeshRenderer(MeshType::kCube);
	mr->setTexture(RenderEngine::GetInstance()->GetTextureID("Assets/Textures/Concrete.jpg"));
	mr->setProgram(RenderEngine::GetInstance()->GetShaderProgram());
	go->transform->position = glm::vec3(0.0f, 0.0f, 0.0f);
	go->AddComponent(mr);
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