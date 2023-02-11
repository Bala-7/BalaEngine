#include "RenderEngine.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Core/Time.h"
#include <fstream>
#include <algorithm>

RenderEngine::RenderEngine()
{
	p_Instance = this;
	renderDelayMilliseconds = (1.0f / config.FPS_MAX);

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
	LoadConfigFile();
	InitializeConfigValues();

	InitGLFW();
	window = CreateWindow();
	InitGLEW();

	InitGame();

	LoadFont("Assets/Fonts/Arial.ttf");

	Debug::Log("Render Engine initialized!");
}

void RenderEngine::Update()
{
	// Clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(1.0, 1.0, 0.0, 1.0);//clear yellow
	//glClearColor(0.0, 0.0, 0.0, 1.0);//clear black

	for (int i = LAYER_MAX - 1; i >= 0; --i)
	{
		layerList[i]->Update();
	}
}

void RenderEngine::UpdateUI()
{
	for (int i = 0; i < uiRenderers.size(); ++i)
	{
		uiRenderers[i]->Update();
	}
}

void RenderEngine::UpdateDebug()
{
	for (int i = 0; i < uiDebugRenderers.size(); ++i)
	{
		uiDebugRenderers[i]->Update();
	}
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

GLuint RenderEngine::GetTextShaderProgram()
{
	return textShaderProgram;
}

std::map<char, RenderEngine::Character> RenderEngine::GetCharacterList()
{
	return Characters;
}

void RenderEngine::AddSpriteToRenderList(SpriteRenderer* spriteRenderer)
{
	layerList[spriteRenderer->GetLayer()]->GetRenderList()->push_back(spriteRenderer);
}

void RenderEngine::AddTextToUI(UITextRenderer* textRenderer)
{
	uiRenderers.push_back(textRenderer);
}

void RenderEngine::AddTextToDebugUI(UITextRenderer* textRenderer)
{
	uiDebugRenderers.push_back(textRenderer);
}

RenderEngine::Config RenderEngine::GetConfiguration()
{
	return config;
}

int RenderEngine::GetTargetFPS()
{
	return config.FPS_MAX;
}

int RenderEngine::GetCurrentFPS()
{
	float lastFrameTimeSeconds = Time::GetLastFrameTime();
	return (lastFrameTimeSeconds > 0) ? 1 / lastFrameTimeSeconds : 1000;
}

glm::vec3 RenderEngine::GetEnvironmentLight()
{
	return environmentLight;
}

void RenderEngine::SetEnvironmentLight(glm::vec3 value)
{
	environmentLight = value;
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

	camera = new Camera(100.0f, 0.1f, 100.0f, glm::vec3(0.0f, 0.0f, 6.0f));
	glm::mat4 projection = glm::ortho(0.0f, (float) config.WINDOW_SIZE_X, (float) config.WINDOW_SIZE_Y, 0.0f, -1.0f, 1.0f);
	ShaderLoader shaderLoader;
	shaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_PATH.c_str(), config.FRAGMENT_SHADER_PATH.c_str());
	textShaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_TEXT_PATH.c_str(), config.FRAGMENT_SHADER_TEXT_PATH.c_str());
}

GLFWwindow* RenderEngine::CreateWindow()
{
	GLFWwindow* window = glfwCreateWindow(config.WINDOW_SIZE_X, config.WINDOW_SIZE_Y, "BalaEngine", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, config.WINDOW_SIZE_X, config.WINDOW_SIZE_Y);

	return window;
}

void RenderEngine::LoadFont(const char* path)
{
	if (FT_Init_FreeType(&ft))
	{
		Debug::LogError("ERROR::FREETYPE: Could not init FreeType Library");
		return;
	}

	if (FT_New_Face(ft, path, 0, &ftFace))
	{
		Debug::LogError("ERROR::FREETYPE: Failed to load font");
		return;
	}

	FT_Set_Pixel_Sizes(ftFace, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			ftFace->glyph->bitmap.width,
			ftFace->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			ftFace->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
			glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
			ftFace->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(ftFace);
	FT_Done_FreeType(ft);

	Debug::Log("Font loaded!");
}

void RenderEngine::LoadConfigFile()
{
	// std::ifstream is RAII, i.e. no need to call close
	std::ifstream cFile("Assets/Config/RenderConfig.txt");
	if (cFile.is_open())
	{
		std::string line;
		while (getline(cFile, line))
		{
			line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
			if (line.empty() || line[0] == '#')
			{
				continue;
			}
			
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);

			auto value = line.substr(delimiterPos + 1);
			ParseValue(name, value);
		}
	}
	else
	{
		
	}
}

void RenderEngine::ParseValue(std::string name, std::string value)
{
	config.configValues[name] = value;
}

void RenderEngine::InitializeConfigValues()
{
	config.FPS_MAX = std::stoi(config.configValues["FPS_MAX"]);
	config.WINDOW_SIZE_X = std::stoi(config.configValues["RESOLUTION_X"]);
	config.WINDOW_SIZE_Y = std::stoi(config.configValues["RESOLUTION_Y"]);

	config.FONTS_PATH = config.configValues["FONTS_PATH"];
	config.SHADERS_PATH = config.configValues["SHADERS_PATH"];

	config.VERTEX_SHADER_PATH = config.configValues["VERTEX_SHADER_PATH"];
	config.FRAGMENT_SHADER_PATH = config.configValues["FRAGMENT_SHADER_PATH"];

	config.VERTEX_SHADER_TEXT_PATH = config.configValues["VERTEX_SHADER_TEXT_PATH"];
	config.FRAGMENT_SHADER_TEXT_PATH = config.configValues["FRAGMENT_SHADER_TEXT_PATH"];
}


