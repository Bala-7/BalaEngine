#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos);
	Camera(GLfloat zoom, GLfloat nearPlane, GLfloat farPlane, glm::vec3 camPos);
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getCameraPosition();


	void setYaw(float newYaw) { yaw = newYaw; }
	void setPitch(float newPitch) { pitch = newPitch; }

	//void MouseControl(GLfloat xChange, GLfloat yChange);

	void Update();

	void OnKeyInput(bool* keys, float deltaTime);
	void OnMouseMovement(int x, int y, float deltaTime);

	glm::mat4 CalculateViewMatrix();

private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 cameraPos;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw = 0.0f;
	float pitch = 0.0f;

	GLfloat movementSpeed;
	GLfloat rotationSpeed;
	
};

