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
#include "Engine/Editor/editor.h"
#include "Engine/Core/SceneGraph.h"
#include "Engine/Core/SceneNode.h"
#include "Engine/Rendering/TexturedCubeMapFBO.h"
#include "Engine/Rendering/SkyboxRenderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

void CreateCube3D();
void CreateSprites2D();
void CreateModel();
void CreateSceneGraph();

RenderEngine* renderEngine;
GameplayEngine* gameplayEngine;
Editor* editor;
SceneGraph* sceneGraph;

MeshRenderer* mr;
ModelRenderer* modelRenderer;
Light* light;
Model* model;

// Input
bool keys[1024];
GLfloat lastX;
GLfloat lastY;
GLfloat xChange;
GLfloat yChange;
bool mouseFirstMoved = true;

int main()
{
	renderEngine = new RenderEngine();
	renderEngine->Initialize();

	gameplayEngine = new GameplayEngine();
	gameplayEngine->Initialize();

	GLFWwindow* window = renderEngine->GetWindow();
	
	editor = Editor::Instance();
	editor->Initialize();

	CoInitialize(nullptr);

	/*Shader ourShader("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");

	Model ourModel("resources/objects/backpack/backpack.obj");*/

	//CreateCube3D();
	//CreateSprites2D();
	CreateSceneGraph();

	glfwSetKeyCallback(window, key_callback);	// Input callback
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPositionCallback);

	// RendererInitialization();	// Triangle renderer

	auto currentTime = Time::Now();
	float t = 0.0f;
	const float dt = 1.0f / renderEngine->GetTargetFPS();
	float accumulator = 0.0f;

	// TEMP : Shadowmap variables
	// Light matrices
	glm::vec3 lightDirection = glm::vec3(0.0f, -1.0f, -1.0f);
	float nearPlane = 0.01f;   // Near clipping plane
	float farPlane = 100.0f;  // Far clipping plane
	glm::mat4 lightProjectionMatrix = glm::ortho(-35.0f, 35.f, -35.f, 35.f, nearPlane, farPlane);
	glm::mat4 lightViewMatrix = glm::lookAt(lightDirection, glm::vec3(0.4f, -1.4f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightViewProjectionMatrix = lightProjectionMatrix * lightViewMatrix;
	
	// Shadowmap framebuffer and texture
	ShaderLoader shaderLoader;
	GLuint screenQuadShader = shaderLoader.createProgram("Assets/Shaders/ScreenQuadShader.vs", "Assets/Shaders/ScreenQuadShader.fs");
	GLuint screenQuadVAO;
	glGenVertexArrays(1, &screenQuadVAO);
	glBindVertexArray(screenQuadVAO);

	// Define the vertex positions and texture coordinates for the quad
	float quadVertices[] = {
		// Positions        // Texture Coordinates
		-1.0f,  1.0f,  0.0f,  0.0f, 1.0f, // Top-left
		 1.0f,  1.0f,  0.0f,  1.0f, 1.0f, // Top-right
		-1.0f, -1.0f,  0.0f,  0.0f, 0.0f, // Bottom-left
		 1.0f, -1.0f,  0.0f,  1.0f, 0.0f, // Bottom-right
	};
	GLubyte quadIndices[] = { 0,1,2, // first triangle (bottom left - top left - top right)
					 1,3,2 };

	// Generate and bind a vertex buffer object (VBO) for the quad vertices
	GLuint quadVBO;
	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// Specify the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture coordinates
	glEnableVertexAttribArray(1);

	// Unbind the VAO and VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	// \TEMP\

	while (!renderEngine->ShouldClose())
	{
		auto newTime = Time::Now();
		float frameTime = (newTime.count() - currentTime.count()) / 1000000.0f;
		Time::SetLastFrameTime(frameTime);
		currentTime = newTime;
		
		accumulator += frameTime;

		glfwPollEvents();

		renderEngine->GetCamera()->OnKeyInput(keys, frameTime);
		if(keys[GLFW_KEY_LEFT_SHIFT])
			renderEngine->GetCamera()->OnMouseMovement(xChange, yChange, frameTime);
		
		xChange = 0.0f;
		yChange = 0.0f;

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

		renderEngine->UseWindow(window);
		renderEngine->RenderSceneView(sceneGraph);
		renderEngine->RenderPlayView(sceneGraph);
		editor->DrawEditorWindows();

		/* Test for drawing shadowmap framebuffer onto a quad. */
		/*glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(screenQuadShader);
		glUniform1i(glGetUniformLocation(screenQuadShader, "screenTexture"), 1);
		glBindVertexArray(screenQuadVAO);
		glDisable(GL_DEPTH_TEST);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, quadIndices);*/
		/* End of text for drawing shadowmap*/
		
		
		//renderEngine->Update();
		// \Scene Rendering code\
			
		// GUI Rendering code
		//renderEngine->UpdateUI();
		//renderEngine->UpdateDebug();
		// \GUI Rendering code
		renderEngine->SetEnvironmentLight(glm::vec3(editor->color[0], editor->color[1], editor->color[2]));
		renderEngine->SetDirectionalLightDirection(editor->directionalLightDirection);
		glfwSwapBuffers(window);

	}

	CoUninitialize();

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

	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);
}

// GLFW callback for mouse button events
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) 
{
	
}

// GLFW callback for mouse movement events
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) 
{
	if (editor->IsMouseOverSceneView())
	{
		if (mouseFirstMoved)
		{
			lastX = xpos;
			lastY = ypos;
			mouseFirstMoved = false;
		}

		xChange = xpos - lastX;
		yChange = lastY - ypos;	// Invert this operation to invert mouse Y movement

		lastX = xpos;
		lastY = ypos;
	}

	// Pass the event to ImGui for its internal handling
	ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
	
}


// -----------------------------------

void CreateCube3D()
{
	GameObject* lightGO = new GameObject();
	light = new Light(Light::LightType::Point,
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)),
		1.0f, 25.0f);
	
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
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)),
		1.0f, 25.0f);

	GameObject* meshGO = new GameObject();
	modelRenderer = new ModelRenderer("Assets/Models/Samus/Samus/Samus_small.obj");

	meshGO->transform->position = glm::vec3(0.05f, -2.0f, 5.0f);
	meshGO->transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	meshGO->transform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	meshGO->AddComponent(modelRenderer);

	editor->SetDisplayedGameObject(meshGO);

	//model = new Model("Assets/Models/example.obj");


}

void CreateSceneGraph()
{
	sceneGraph = new SceneGraph();
	
	// Light
	GameObject* lightGO = new GameObject("Point Light");
	sceneGraph->GetRootNode()->AddChild(new SceneNode(lightGO));
	ModelRenderer* lightModel = new ModelRenderer("Assets/Models/Lightbulb/Lightbulb.obj");
	light = new Light(Light::LightType::Point,
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f,
		12.5f, 17.5f,
		1.0f, 25.0f);
	lightGO->transform->position = glm::vec3(0.0f, 1.0f, 0.2f);
	lightGO->AddComponent(light);

	// Second light
	GameObject* lightGO2 = new GameObject("Point Light 2");
	sceneGraph->GetRootNode()->AddChild(new SceneNode(lightGO2));
	Light* light2 = new Light(Light::LightType::Point,
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
		1.0f, 0.09f, 0.032f,
		12.5f, 17.5f,
		1.0f, 25.0f);
	lightGO2->transform->position = glm::vec3(3.0f, 1.0f, 0.0f);
	lightGO2->AddComponent(light2);

	// 3D Model
	GameObject* meshGO = new GameObject("Samus Model");
	modelRenderer = new ModelRenderer("Assets/Models/Samus/Samus/Samus_small.obj");
	meshGO->transform->position = glm::vec3(0.00f, 0.0f, 0.0f);
	meshGO->transform->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	meshGO->transform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	meshGO->AddComponent(modelRenderer);
	editor->SetDisplayedGameObject(meshGO);

	GameObject* fs0 = new GameObject("Firelink Shrine 1");
	ModelRenderer* fs0MR = new ModelRenderer("Assets/Models/Firelink Shrine/m0000B2A10.obj");
	fs0->AddComponent(fs0MR);

	GameObject* terrainGO = new GameObject("Terrain");
	ModelRenderer* terrainModel = new ModelRenderer("Assets/Models/DefaultTerrain.obj");
	terrainGO->transform->position = glm::vec3(0.00f, -0.15f, 0.0f);
	terrainGO->AddComponent(terrainModel);


	GameObject* wallGO = new GameObject("Wall_1");
	ModelRenderer* wallModel = new ModelRenderer("Assets/Models/DefaultTerrain.obj");
	wallGO->transform->position = glm::vec3(0.00f, 2.35f, -2.5f);
	wallGO->transform->rotation = glm::vec3(90.00f, 0.0f, 0.0f);
	wallGO->AddComponent(wallModel);

	
	SkyboxRenderer* skybox = new SkyboxRenderer();
	skybox->setProgram(RenderEngine::GetInstance()->GetSkyboxShaderProgram());
	GameObject* skyboxGO = new GameObject("Skybox");
	skyboxGO->AddComponent(skybox);

	sceneGraph->GetRootNode()->AddChild(new SceneNode(meshGO));
	//sceneGraph->GetRootNode()->AddChild(new SceneNode(fs0));
	sceneGraph->GetRootNode()->AddChild(new SceneNode(terrainGO));
	sceneGraph->GetRootNode()->AddChild(new SceneNode(wallGO));
	sceneGraph->GetRootNode()->AddChild(new SceneNode(skyboxGO));

	editor->sceneGraph = sceneGraph;
}