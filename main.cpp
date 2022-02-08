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

int main()
{
	InitGLFW();
	GLFWwindow* window = CreateWindow();
	InitGLEW();
	
	glfwSetKeyCallback(window, key_callback);	// Input callback

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Rendering code
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// \Rendering code\

		glfwSwapBuffers(window);
	}


	glfwTerminate();
	return 0;
}

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "BalaEngine", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 800, 600);

	return window;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}