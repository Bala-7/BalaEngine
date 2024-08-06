#pragma once
#include "Engine/Core/Component.h"
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/Camera.h"
#include "Engine/Rendering/RenderableComponent.h"
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
	void setVerticesData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	void setupShaderForDraw();
	void draw();
	void draw(Camera* camera) override;

	GLuint vao, vbo, ebo, texture, program;
	std::vector<Vertex>vertices;
	std::vector<GLuint>indices;

	Camera* renderCamera;
	Shader* shader;

	glm::mat4 modelMatrix;
	glm::vec3 position, scale, rotation;
};

