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

	Shader* shader;
private:

	std::vector<Vertex>vertices;
	std::vector<GLuint>indices;
	glm::mat4 modelMatrix;
	Camera* camera;
	glm::vec3 position, scale, rotation;
	GLuint vao, vbo, ebo, texture, program;
};

