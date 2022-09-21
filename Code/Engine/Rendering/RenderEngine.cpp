#include "RenderEngine.h"
#include "Engine/Debug/Debug.h"

RenderEngine::RenderEngine()
{
	p_Instance = this;

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		SpriteLayer* newLayer = new SpriteLayer();
		layerList.push_back(newLayer);
	}
}

RenderEngine* RenderEngine::p_Instance;

RenderEngine::~RenderEngine()
{
}

void RenderEngine::Initialize()
{
	InitGLFW();
	window = CreateWindow();
	InitGLEW();

	InitGame();

	Debug::Log("Render Engine initialized!");
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

	for (int i = LAYER_MAX - 1; i >= 0; --i)
	{
		layerList[i]->Update();
	}
	

	//go->Update();
	//go2->Update();
}

void RenderEngine::Terminate()
{
	glfwTerminate();

	delete camera;
	
	
}

bool RenderEngine::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* RenderEngine::GetWindow()
{
	return window;
}

Camera* RenderEngine::GetCamera()
{
	return camera;
}



GLuint RenderEngine::GetTextureID(const char* fileName)
{
	char* s = new char[strlen(TEXTURES_PATH) + strlen(fileName)];
	strcpy(s, TEXTURES_PATH); strcat(s, fileName);
	return textureLoader.getTextureID(s);
}

GLuint RenderEngine::GetShaderProgram()
{
	return shaderProgram;
}

void RenderEngine::AddSpriteToRenderList(SpriteRenderer* spriteRenderer)
{
	layerList[spriteRenderer->GetLayer()]->GetRenderList()->push_back(spriteRenderer);
	//spritesRenderList.push_back(spriteRenderer);
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
	shaderProgram = shaderLoader.createProgram(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

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
