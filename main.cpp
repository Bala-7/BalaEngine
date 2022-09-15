// GLEW
//#define GLEW_STATIC
//#include<GL/glew.h>
// GLFW
//#include <GLFW/glfw3.h>

#include <iostream>
#include "Code/Engine/Rendering/ShaderLoader.h"
#include "Code/Engine/Rendering/Camera.h"
#include "Code/Engine/Rendering/LightRenderer.h"
#include "Code/Engine/Rendering/TextureLoader.h"
#include "Code/Engine/Rendering/MeshRenderer.h"
#include "Code/Engine/Rendering/SpriteRenderer.h"
#include "Code/Engine/Rendering/RenderEngine.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

RenderEngine* renderEngine;

int main()
{
	renderEngine = new RenderEngine();
	renderEngine->Initialize();

	GLFWwindow* window = renderEngine->GetWindow();

	glfwSetKeyCallback(window, key_callback);	// Input callback
	// RendererInitialization();	// Triangle renderer
	while (!renderEngine->ShouldClose())
	{
		glfwPollEvents();

		// Rendering code
		renderEngine->Update();
		// \Rendering code\

		glfwSwapBuffers(window);
	}


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