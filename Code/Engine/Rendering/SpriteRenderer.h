#pragma once
#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include <GL/glew.h>
#include <vector>
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Engine/Core/Component.h"
#include "Mesh.h"



class SpriteRenderer : public Component
{
public:
	SpriteRenderer();
	~SpriteRenderer();
	
	void Update() override;
	void OnComponentAdded() override;

	
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setProgram(GLuint _program);
	void setTexture(GLuint _textureID);
	void setSprite(int index);
	void setSpriteSheet(const char* fileName, int spriteWidth, int spriteHeight, int sheetCountWidth, int sheetCountHeight);
	void setLayer(int newLayer);
	int GetLayer();

private:
	void setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	void draw();

	std::vector<Vertex>vertices;
	std::vector<GLuint>indices;
	glm::mat4 modelMatrix;
	Camera* camera;
	glm::vec3 position, scale;
	GLuint vao, vbo, ebo, texture, program;

	int layer;
	int spriteWidth, spriteHeight;
	int sheetCountWidth, sheetCountHeight;	// Number of horizontal and vertical sprites
};

#endif // !SPRITERENDERER_H