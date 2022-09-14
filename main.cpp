// GLEW
#define GLEW_STATIC
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "ShaderLoader.h"
#include "Camera.h"
#include "LightRenderer.h"
#include "TextureLoader.h"
#include "MeshRenderer.h"
#include "SpriteRenderer.h"

Camera* camera;
LightRenderer* light;
MeshRenderer* mesh;
SpriteRenderer* sprite;
SpriteRenderer* sprite2;

void initGame();

void InitGLFW();
void InitGLEW();
GLFWwindow* CreateWindow();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void RenderUpdate();

const int WINDOW_SIZE_X = 1280;
const int WINDOW_SIZE_Y = 720;

const char* VERTEX_SHADER_PATH = "Assets/Shaders/FlatModel.vs";
const char* FRAGMENT_SHADER_PATH = "Assets/Shaders/FlatModel.fs";

const char* SPRITE_VERTEX_SHADER_PATH = "Assets/Shaders/SpriteShader.vs";
const char* SPRITE_FRAGMENT_SHADER_PATH = "Assets/Shaders/SpriteShader.fs";


int main()
{
	InitGLFW();
	GLFWwindow* window = CreateWindow();
	InitGLEW();
	
	initGame();

	glfwSetKeyCallback(window, key_callback);	// Input callback
	// RendererInitialization();	// Triangle renderer
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Rendering code
		RenderUpdate();
		// \Rendering code\

		glfwSwapBuffers(window);
	}


	glfwTerminate();

	delete camera;
	delete light;

	return 0;
}

// Game

void initGame()
{
	// Enable Depth Test, so only pixels in the front are drawn
	glEnable(GL_DEPTH_TEST);

	// Create the ShaderLoader class, and creates shader programs, passing the vertex and fragment shaders to them
	/*ShaderLoader shaderLoader;
	GLuint flatShaderProgram = shaderLoader.createProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	GLuint texturedShaderProgram = shaderLoader.createProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");

	// Load textures
	TextureLoader textureLoader;
	GLuint meshTexture = textureLoader.getTextureID("Assets/Textures/icon.jpg");

	// Create the camera
	camera = new Camera(45.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0.1f, 100.0f, glm::vec3(0.0f, 0.0f, 6.0f));

	// Create the light
	light = new LightRenderer(MeshType::kTriangle, camera);
	light->setProgram(flatShaderProgram);
	light->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));*/

	// Create the mesh
	/*mesh = new MeshRenderer(MeshType::kQuad, camera);
	mesh->setProgram(texturedShaderProgram);
	mesh->setTexture(meshTexture);
	mesh->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	mesh->setScale(glm::vec3(3.0f));*/


	camera = new Camera(45.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0.1f, 100.0f, glm::vec3(0.0f, 0.0f, 6.0f));
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	ShaderLoader shaderLoader;
	GLuint spriteShaderProgram = shaderLoader.createProgram(SPRITE_VERTEX_SHADER_PATH, SPRITE_FRAGMENT_SHADER_PATH);
	GLuint texturedShaderProgram = shaderLoader.createProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");

	// Load textures
	TextureLoader textureLoader;
	//GLuint meshTexture = textureLoader.getTextureID("Assets/Textures/Character.png");
	GLuint meshTexture = textureLoader.getTextureID("Assets/Textures/PokemonBlue_Player_Spritesheet.png");

	sprite = new SpriteRenderer(camera);
	sprite->setProgram(texturedShaderProgram);
	sprite->setTexture(meshTexture);
	sprite->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	sprite->setScale(glm::vec3(1.0f));

	sprite2 = new SpriteRenderer(camera);
	sprite2->setProgram(texturedShaderProgram);
	sprite2->setTexture(meshTexture);
	sprite2->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	sprite2->setScale(glm::vec3(0.5f));

}

// -- Window and OpenGL initialization

/* Initializes GLFW context to use OpenGL 3.3 with core profile and non-resizable windows. */
void InitGLFW() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void InitGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}
}

GLFWwindow* CreateWindow()
{
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE_X, WINDOW_SIZE_Y, "BalaEngine", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, WINDOW_SIZE_X, WINDOW_SIZE_Y);

	return window;
}

// -----------------------------------

// -- Rendering ----------------------

void RenderUpdate()
{
	// Clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(1.0, 1.0, 0.0, 1.0);//clear yellow
	
	//mesh->draw();
	sprite->draw();
	sprite2->draw();
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