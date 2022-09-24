#include "RenderEngine.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Core/Time.h"

RenderEngine::RenderEngine()
{
	p_Instance = this;
	renderDelayMilliseconds = (1.0f / MAX_FPS);

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

	LoadFont("Assets/Fonts/Arial.ttf");

	Debug::Log("Render Engine initialized!");
}

void RenderEngine::Update()
{
	// Clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearColor(1.0, 1.0, 0.0, 1.0);//clear yellow

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

int RenderEngine::GetTargetFPS()
{
	return MAX_FPS;
}

int RenderEngine::GetCurrentFPS()
{
	float lastFrameTimeSeconds = Time::GetLastLoopTime() / 1000;
	return (lastFrameTimeSeconds > 0) ? 1 / lastFrameTimeSeconds : 1000;
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
	textShaderProgram = shaderLoader.createProgram(VERTEX_SHADER_PATH_TEXT, FRAGMENT_SHADER_PATH_TEXT);
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
