#include "RenderEngine.h"

RenderEngine::RenderEngine()
{
}

RenderEngine::~RenderEngine()
{
}

void RenderEngine::Initialize()
{
	InitGLFW();
	window = CreateWindow();
	InitGLEW();

	InitGame();
}

void RenderEngine::Update()
{
	// Clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(1.0, 1.0, 0.0, 1.0);//clear yellow

	//mesh->draw();
	//sprite->draw();
	//sprite2->draw();

	go->Update();
	go2->Update();
}

void RenderEngine::Terminate()
{
	glfwTerminate();

	delete camera;
	delete light;
	delete sprite;
	delete sprite2;
}

bool RenderEngine::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* RenderEngine::GetWindow()
{
	return window;
}

void RenderEngine::InitGLFW()
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

void RenderEngine::InitGLEW()
{
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}
}

void RenderEngine::InitGame()
{
	// Enable Depth Test, so only pixels in the front are drawn
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera = new Camera(45.0f, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0.1f, 100.0f, glm::vec3(0.0f, 0.0f, 6.0f));
	glm::mat4 projection = glm::ortho(0.0f, (float) WINDOW_SIZE_X, (float) WINDOW_SIZE_Y, 0.0f, -1.0f, 1.0f);
	ShaderLoader shaderLoader;
	GLuint texturedShaderProgram = shaderLoader.createProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");

	// Load textures
	TextureLoader textureLoader;
	GLuint rpgCharacterSpritesheet = textureLoader.getTextureID("Assets/Textures/Character.png");
	GLuint pokemonSpritesheet = textureLoader.getTextureID("Assets/Textures/PokemonBlue_Player_Spritesheet.png");

	

	sprite = new SpriteRenderer(camera);
	sprite->setProgram(texturedShaderProgram);
	sprite->setSpriteSheet(rpgCharacterSpritesheet, 32, 32, 3, 4);
	sprite->setSprite(10);
	sprite->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	sprite->setScale(glm::vec3(1.0f));

	sprite2 = new SpriteRenderer(camera);
	sprite2->setProgram(texturedShaderProgram);
	//sprite2->setTexture(meshTexture);
	sprite2->setSpriteSheet(pokemonSpritesheet, 16, 16, 10, 1);
	sprite2->setSprite(1);
	sprite2->setPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
	sprite2->setScale(glm::vec3(1.0f));

	go = new GameObject();
	go->AddComponent(sprite);

	go2 = new GameObject();
	go2->AddComponent(sprite2);
}

GLFWwindow* RenderEngine::CreateWindow()
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
