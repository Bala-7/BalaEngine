#pragma once
#include <vector>
#include "Camera.h"
#include "LightRenderer.h"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Engine/Core/Component.h"
#include "Engine/Rendering/Shader.h"

struct Material
{
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
	float shininess = 32.0f;
};

class MeshRenderer : public Component
{

public:
	MeshRenderer(MeshType modelType);
	~MeshRenderer();

	void Update() override;
	void OnComponentAdded() override;

	void draw();
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setRotation(glm::vec3 _rotation);
	void setProgram(GLuint _program);
	void setTexture(GLuint _textureID);

	Material* GetMaterial();

	Shader* shader;
private:
	Material* _material;
	std::vector<Vertex>vertices;
	std::vector<GLuint>indices;
	glm::mat4 modelMatrix;
	Camera* camera;
	glm::vec3 position, scale, rotation;
	GLuint vao, vbo, ebo, texture, program;
};

