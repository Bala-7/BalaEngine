#pragma once
#include <GL/glew.h>
#include <vector>
#include "Camera.h"
#include "LightRenderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class SpriteRenderer
{
public:
	SpriteRenderer(Camera* _camera);
	~SpriteRenderer();
	
	void setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

	void draw();
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setProgram(GLuint _program);
	void setTexture(GLuint _textureID);
	void setSprite(int index);
	void setSpriteSheet(GLuint _textureID, int spriteWidth, int spriteHeight, int sheetCountWidth, int sheetCountHeight);

private:
	std::vector<Vertex>vertices;
	std::vector<GLuint>indices;
	glm::mat4 modelMatrix;
	Camera* camera;
	glm::vec3 position, scale;
	GLuint vao, vbo, ebo, texture, program;

	int spriteWidth, spriteHeight;
	int sheetCountWidth, sheetCountHeight;	// Number of horizontal and vertical sprites
};

