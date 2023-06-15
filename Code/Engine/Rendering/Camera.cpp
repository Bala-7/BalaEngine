#include "Camera.h"
#include "RenderEngine.h"

Camera::Camera(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos) {
	cameraPos = camPos;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	up = worldUp;
	yaw = 0;
	pitch = 0;

	movementSpeed = 5.0f;
	rotationSpeed = 10.0f;

	viewMatrix = CalculateViewMatrix();
	projectionMatrix = glm::perspective(FOV, width / height, nearPlane, farPlane);
}

Camera::Camera(GLfloat zoom, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos) {
	cameraPos = camPos;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	up = worldUp;
	yaw = 0;
	pitch = 0;
	movementSpeed = 5.0f;
	rotationSpeed = 10.0f;
	viewMatrix = glm::lookAt(cameraPos, front, up);
	
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


void Camera::Update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

	viewMatrix = CalculateViewMatrix();
}

void Camera::OnKeyInput(bool* keys, float deltaTime)
{
	if (keys[GLFW_KEY_LEFT_SHIFT]) 
	{
		if (keys[GLFW_KEY_W])
		{
			cameraPos += front * movementSpeed * deltaTime;
		}
		if (keys[GLFW_KEY_S])
		{
			cameraPos -= front * movementSpeed * deltaTime;
		}
		if (keys[GLFW_KEY_D])
		{
			cameraPos += right * movementSpeed * deltaTime;
		}
		if (keys[GLFW_KEY_A])
		{
			cameraPos -= right * movementSpeed * deltaTime;
		}

		viewMatrix = CalculateViewMatrix();
	}
}

void Camera::OnMouseMovement(int x, int y, float deltaTime)
{
	x *= rotationSpeed;
	y *= rotationSpeed;
	
	yaw += x * deltaTime;
	pitch += y * deltaTime;

	if (pitch < -89.0f) pitch = -89.0f;
	if (pitch > 89.0f) pitch = 89.0f;

	Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + front, up);
	
}
