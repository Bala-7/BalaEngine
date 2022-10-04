#include "Camera.h"
#include "RenderEngine.h"

Camera::Camera(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos) {
	cameraPos = camPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	viewMatrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
	projectionMatrix = glm::perspective(FOV, width / height, nearPlane, farPlane);
}

Camera::Camera(GLfloat zoom, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos) {
	cameraPos = camPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	viewMatrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
	
	RenderEngine::Config renderConfiguration = RenderEngine::GetInstance()->GetConfiguration();
	float scale = 2.0f * zoom;
	float left = -renderConfiguration.WINDOW_SIZE_X / scale;
	float right = renderConfiguration.WINDOW_SIZE_X / scale;
	float top = renderConfiguration.WINDOW_SIZE_Y / scale;
	float bottom = -renderConfiguration.WINDOW_SIZE_Y / scale;
	projectionMatrix = glm::ortho(left, right, bottom, top, -1000.0f, 1000.0f);
}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix() {
	return viewMatrix;
}
glm::mat4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}
glm::vec3 Camera::getCameraPosition() {
	return cameraPos;
}