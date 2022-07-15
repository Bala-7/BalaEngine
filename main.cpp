// GLEW
#define GLEW_STATIC
#include<GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

void InitGLFW();
void InitGLEW();
GLFWwindow* CreateWindow();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void RenderUpdate();
void RendererInitialization();

const int WINDOW_SIZE_X = 1280;
const int WINDOW_SIZE_Y = 720;

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

GLfloat vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };
GLuint shaderProgram;
GLuint VAO;

int main()
{
	InitGLFW();
	GLFWwindow* window = CreateWindow();
	InitGLEW();
	
	glfwSetKeyCallback(window, key_callback);	// Input callback
	RendererInitialization();
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Rendering code
		RenderUpdate();
		// \Rendering code\

		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}

// -- Window and OpenGL initialization

/* Initializes GLFW context to use OpenGL 3.3 with core profile and non-resizable windows. */
void InitGLFW() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void RenderUpdate()
{
	// Clear color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Triangle
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

}

void RendererInitialization()
{
	// -------- Triangle -----------
	

	// Creates Vertex Array Object to store calls to VertexAttribute methods
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create Vertex Buffer Object to store vertices and send them to the GPU
	GLuint VBO;
	glGenBuffers(1, &VBO);	// Generates the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);	// Binds the buffer as a GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Fills the buffer with vertex data

	// Vertex Shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// Fragment Shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// Shader Program (linked version of multiple shaders combined)
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" <<
			infoLog << std::endl;
	}
	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//4. Unbind the VAO
	glBindVertexArray(0);


	// ------ Triangle End ---------
}