#pragma once
#include "Engine/Core/Component.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/Camera.h"
#include "Engine/Rendering/RenderableComponent.h"
#include "Engine/Rendering/TexturedCubeMapFBO.h"
#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"

class SkyboxRenderer : public RenderableComponent
{
public:
	SkyboxRenderer();
	void Update();

	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setRotation(glm::vec3 _rotation);
	void setProgram(GLuint _program);
	void setShaderProgram(GLuint _program);
	void setTexture(GLuint textureID);

	void DrawInspector();
	void OnComponentAdded();

	void DrawShadows() override;
	void DrawCubemapShadows() override;

private:
	void setupShaderForDraw();
	void draw();
	void draw(Camera* camera) override;

	std::string OpenFolderDialog();
	void reloadTexture(std::string newPath);


	std::vector<std::string> faces
	{
		"Assets/Textures/skybox/right.jpg",
		"Assets/Textures/skybox/left.jpg",
		"Assets/Textures/skybox/bottom.jpg",
		"Assets/Textures/skybox/top.jpg",
		"Assets/Textures/skybox/front.jpg",
		"Assets/Textures/skybox/back.jpg"
	};

	GLuint vao, vbo, ebo, texture, program;

	TexturedCubeMapFBO* skyboxFBO;

	Camera* renderCamera;
	Shader* shader;

	glm::mat4 modelMatrix;
	glm::vec3 position, scale, rotation;
};

