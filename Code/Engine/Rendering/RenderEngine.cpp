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
	UseWindow(window);
	InitGLEW();

	InitGame();

	LoadFont("Assets/Fonts/Arial.ttf");

	CreateFramebuffer();

	CreateShadowmapFramebuffer();	

	CreateObjectPickingFramebuffer();

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

	delete sceneViewCamera;
}

bool RenderEngine::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* RenderEngine::GetWindow()
{
	return window;
}

GLFWwindow* RenderEngine::GetPlayWindow()
{
	return playWindow;
}

void RenderEngine::UseWindow(GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
}

Camera* RenderEngine::GetCamera()
{
	return sceneViewCamera;
}

GLuint RenderEngine::GetTextureID(const char* fileName, bool fullPath)
{
	if (!fullPath) 
	{
		char* s = new char[strlen(TEXTURES_PATH) + strlen(fileName)];
		strcpy(s, TEXTURES_PATH); strcat(s, fileName);
		return textureLoader.getTextureID(s);
	}
	else
	{
		return textureLoader.getTextureID(fileName);
	}
}



GLuint RenderEngine::GetShaderProgram()
{
	return shaderProgram;
}

GLuint RenderEngine::GetSkyboxShaderProgram()
{
	return skyboxShaderProgram;
}

GLuint RenderEngine::GetShadowShaderProgram()
{
	return shadowShaderProgram;
}

GLuint RenderEngine::GetPickingShaderProgram()
{
	return pickingShaderProgram;
}

GLuint RenderEngine::GetCubeMapShadowShaderProgram()
{
	return shadowCubeMapShaderProgram;
}

GLuint RenderEngine::GetOutlineShaderProgram()
{
	return outlineShaderProgram;
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

glm::vec3 RenderEngine::GetDirectionalLightDirection()
{
	return directionalLightDirection;
}

void RenderEngine::SetDirectionalLightDirection(glm::vec3 value)
{
	directionalLightDirection = value;
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
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	//camera = new Camera(120, 1280, 720, 0.1f, 100.0f, glm::vec3(0.0f, 0.0f, 6.0f));
	sceneViewCamera = new Camera(120, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y, 0.1f, 100.0f, glm::vec3(0.0f, 0.5f, 4.0f));
	playViewCamera = new Camera(120, config.PLAY_VIEW_SIZE_X, config.PLAY_VIEW_SIZE_Y, 0.1f, 100.0f, glm::vec3(-5.0f, 5.0f, 10.0f));
	playViewCamera->setYaw(-60.0f);
	playViewCamera->setPitch(-20.0f);
	playViewCamera->Update();
	//glm::mat4 projection = glm::ortho(0.0f, (float) config.WINDOW_SIZE_X, (float) config.WINDOW_SIZE_Y, 0.0f, -1.0f, 1.0f);
	ShaderLoader shaderLoader;
	shaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_PATH.c_str(), config.FRAGMENT_SHADER_PATH.c_str());
	outlineShaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_PATH_OUTLINE.c_str(), config.FRAGMENT_SHADER_PATH_OUTLINE.c_str());
	skyboxShaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_PATH_SKYBOX.c_str(), config.FRAGMENT_SHADER_PATH_SKYBOX.c_str());
	shadowShaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_SHADOW_PATH.c_str(), config.FRAGMENT_SHADER_SHADOW_PATH.c_str());
	shadowCubeMapShaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_CM_SHADOW_PATH.c_str(), config.FRAGMENT_SHADER_CM_SHADOW_PATH.c_str(), config.GEOMETRY_SHADER_CM_SHADOW_PATH.c_str());
	pickingShaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_PICKING_PATH.c_str(), config.FRAGMENT_SHADER_PICKING_PATH.c_str());
	textShaderProgram = shaderLoader.createProgram(config.VERTEX_SHADER_TEXT_PATH.c_str(), config.FRAGMENT_SHADER_TEXT_PATH.c_str());
}

GLFWwindow* RenderEngine::CreateWindow()
{
	// Set window hints
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(config.WINDOW_SIZE_X, config.WINDOW_SIZE_Y, "BalaEngine", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glViewport(0, 0, config.WINDOW_SIZE_X, config.WINDOW_SIZE_Y);

	return window;
}


GLFWwindow* RenderEngine::CreatePlayWindow()
{
	// Set window hints
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWwindow* playWindow = glfwCreateWindow(config.WINDOW_SIZE_X, config.WINDOW_SIZE_Y, "Game", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glViewport(0, 0, config.WINDOW_SIZE_X, config.WINDOW_SIZE_Y);
	return playWindow;
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

	config.SCENE_VIEW_SIZE_X = std::stoi(config.configValues["SCENE_VIEW_SIZE_X"]);
	config.SCENE_VIEW_SIZE_Y = std::stoi(config.configValues["SCENE_VIEW_SIZE_Y"]);

	config.PLAY_VIEW_SIZE_X = std::stoi(config.configValues["PLAY_VIEW_SIZE_X"]);
	config.PLAY_VIEW_SIZE_Y = std::stoi(config.configValues["PLAY_VIEW_SIZE_Y"]);

	config.FONTS_PATH = config.configValues["FONTS_PATH"];
	config.SHADERS_PATH = config.configValues["SHADERS_PATH"];

	config.VERTEX_SHADER_PATH = config.configValues["VERTEX_SHADER_PATH"];
	config.FRAGMENT_SHADER_PATH = config.configValues["FRAGMENT_SHADER_PATH"];

	config.VERTEX_SHADER_PATH_OUTLINE = config.configValues["VERTEX_SHADER_PATH_OUTLINE"];
	config.FRAGMENT_SHADER_PATH_OUTLINE = config.configValues["FRAGMENT_SHADER_PATH_OUTLINE"];

	config.VERTEX_SHADER_PATH_SKYBOX = config.configValues["VERTEX_SHADER_PATH_SKYBOX"];
	config.FRAGMENT_SHADER_PATH_SKYBOX = config.configValues["FRAGMENT_SHADER_PATH_SKYBOX"];

	config.VERTEX_SHADER_SHADOW_PATH = config.configValues["VERTEX_SHADER_SHADOWS_PATH"];
	config.FRAGMENT_SHADER_SHADOW_PATH = config.configValues["FRAGMENT_SHADER_SHADOWS_PATH"];

	config.VERTEX_SHADER_PICKING_PATH = config.configValues["VERTEX_SHADER_PICKING_PATH"];
	config.FRAGMENT_SHADER_PICKING_PATH = config.configValues["FRAGMENT_SHADER_PICKING_PATH"];

	config.VERTEX_SHADER_CM_SHADOW_PATH = config.configValues["VERTEX_SHADER_CM_SHADOWS_PATH"];
	config.FRAGMENT_SHADER_CM_SHADOW_PATH = config.configValues["FRAGMENT_SHADER_CM_SHADOWS_PATH"];
	config.GEOMETRY_SHADER_CM_SHADOW_PATH = config.configValues["GEOMETRY_SHADER_CM_SHADOWS_PATH"];


	config.VERTEX_SHADER_TEXT_PATH = config.configValues["VERTEX_SHADER_TEXT_PATH"];
	config.FRAGMENT_SHADER_TEXT_PATH = config.configValues["FRAGMENT_SHADER_TEXT_PATH"];
}

void RenderEngine::CreateShadowmapFramebuffer()
{
	shadowMap = new ShadowMapFBO();
	shadowMap->Init();

	shadowCubeMap = new ShadowCubeMapFBO();
	shadowCubeMap->Init();
}

void RenderEngine::CreateObjectPickingFramebuffer()
{
	// Create a framebuffer object
	glGenFramebuffers(1, &pickingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, pickingFBO);


	// Create a texture to store the color information
	glGenTextures(1, &pickingTexture);
	glBindTexture(GL_TEXTURE_2D, pickingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickingTexture, 0);

	// Create a renderbuffer object to store the depth information
	glGenRenderbuffers(1, &pickingDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, pickingDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickingDepthBuffer);

	// Check framebuffer completeness
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Debug::LogError("Framebuffer is not complete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderEngine::CreateFramebuffer()
{
	// Scene window framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);


	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.SCENE_VIEW_SIZE_X, config.SCENE_VIEW_SIZE_Y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	GLint stencil;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &stencil);
	printf("Stencil Attachment: %d\n", stencil);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || stencil != GL_RENDERBUFFER)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";


	// Play window framebuffer
	glGenFramebuffers(1, &playWindowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, playWindowFBO);


	glGenTextures(1, &playWindowTexture);
	glBindTexture(GL_TEXTURE_2D, playWindowTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, config.PLAY_VIEW_SIZE_X, config.PLAY_VIEW_SIZE_Y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, playWindowTexture, 0);

	glGenRenderbuffers(1, &playWindowRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, playWindowRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, config.PLAY_VIEW_SIZE_X, config.PLAY_VIEW_SIZE_Y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, playWindowRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

// here we bind our framebuffer
void RenderEngine::BindFramebuffer(GLuint fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, 854, 480);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

// here we unbind our framebuffer
void RenderEngine::UnbindFramebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// and we rescale the buffer, so we're able to resize the window
void RenderEngine::RescaleFramebuffer(float width, float height)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}


void RenderEngine::RenderSceneView(SceneGraph* scene)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Shadow pass
	CalculateLightViewMatrices();

	// Shadow map
	shadowMap->Bind();
	scene->DrawShadows();
	shadowMap->Unbind();

	shadowCubeMap->Bind();
	scene->DrawCubemapShadows();
	shadowCubeMap->Unbind();

	
	// Light pass
	BindFramebuffer(FBO);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	
	
	scene->Draw(sceneViewCamera);

	// Debugging
	/* This is okay
	GLint stencil = 0;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &stencil);
	printf("Stencil Attachment: %d\n", stencil);*/

	/* This is also okay
	GLint stencilBits = 0;
	glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
	printf("Stencil Buffer Bits: %d\n", stencilBits);

	GLint stencilValue = -1;
	glReadPixels(300, 300, 1, 1, GL_STENCIL_INDEX, GL_INT, &stencilValue);
	printf("Stencil Value at (%d, %d): %d\n", 300, 300, stencilValue);*/

	// \Debugging\

	// Outline render pass
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(1.0f, 1.0f); // Push fragments back
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // Render only back faces
	scene->DrawOutline(sceneViewCamera);
	glCullFace(GL_BACK);  // Restore normal culling
	//glDisable(GL_POLYGON_OFFSET_FILL);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	UnbindFramebuffer();

	// Object picking pass
	BindFramebuffer(pickingFBO);
	scene->DrawPicking();
	UnbindFramebuffer();

	

	shadowCubeMap->RenderCubemapFaceToTexture();
}

// @TODO: Make the play view to be in a separate window
void RenderEngine::RenderPlayView(SceneGraph* scene)
{
	// Shadow pass
	CalculateLightViewMatrices();

	shadowMap->Bind();
	scene->DrawShadows();
	shadowMap->Unbind();

	// Light pass
	BindFramebuffer(playWindowFBO);
	scene->Draw(playViewCamera);
	UnbindFramebuffer();
}

void RenderEngine::CalculateLightViewMatrices()
{
	float nearPlane = 0.01f;   // Near clipping plane
	float farPlane = 100.0f;  // Far clipping plane
	glm::vec3 lightTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	float lightDistance = 10.0f;
	glm::vec3 lightPosition = lightTarget - glm::normalize(directionalLightDirection) * lightDistance;
	float orthographicSize = 3.0f;
	glm::mat4 lightProjectionMatrix = glm::ortho(-orthographicSize, orthographicSize, -orthographicSize, orthographicSize, nearPlane, farPlane);
	//glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightViewMatrix = glm::lookAt(lightTarget - directionalLightDirection, lightTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	lightViewProjectionMatrix = lightProjectionMatrix * lightViewMatrix;
}

void RenderEngine::StartPlayMode()
{
	playWindow = CreatePlayWindow();
	//UseWindow(playWindow);
}

