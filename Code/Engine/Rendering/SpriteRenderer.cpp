#include "SpriteRenderer.h"
#include "glm/glm.hpp"

SpriteRenderer::SpriteRenderer(Camera* _camera)
{
	camera = _camera;
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);

	setQuadData(vertices, indices);

	// Generate and bind VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Set attributes: position(0) and texture(1)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::texCoords)));

	// Unbind Buffers and vertexArray
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Update()
{
	draw();
}

void SpriteRenderer::setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	std::vector<Vertex> _vertices = {
		{
			{ -0.5f, -0.5f, 0.0f }, // Position
			{ 0.0f, 0.0f, 1.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 0.0, 0.0 } // Texture Coordinate
		}, // 0
		{
			{ 0.5f, -0.5f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.1, 0.0 }
		}, // 1
		{
			{ 0.5f, 0.5f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.1, 1.0 }
		}, // 2
		{
			{ -0.5f, 0.5f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0, 1.0 }
		}, // 3
	};
	std::vector<uint32_t> _indices = { 0, 1, 2, 0, 2, 3 };

	vertices.clear(); indices.clear();

	vertices = _vertices;
	indices = _indices;
}


void SpriteRenderer::draw()
{
	// Set the model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = translationMatrix * scaleMatrix;

	// vp matrix is the multiplied view and projection amtrices
	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();

	// Send the data to the shader program
	glUseProgram(this->program);
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Bind the VAO and draw object
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind VertexArray for safety
	glBindVertexArray(0);
}

// setters
void SpriteRenderer::setTexture(GLuint textureID)
{
	texture = textureID;
}

void SpriteRenderer::setSpriteSheet(GLuint _textureID, int _spriteWidth, int _spriteHeight, int _sheetCountWidth, int _sheetCountHeight)
{
	texture = _textureID;
	spriteWidth = _spriteWidth;
	spriteHeight = _spriteHeight;
	sheetCountWidth = _sheetCountWidth;
	sheetCountHeight = _sheetCountHeight;
}

void SpriteRenderer::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
}

void SpriteRenderer::setPosition(glm::vec3 _position)
{
	this->position = _position;
}

void SpriteRenderer::setProgram(GLuint _program)
{
	this->program = _program;
}

void SpriteRenderer::setSprite(int index)
{
	int cellX = (index % sheetCountWidth);
	int cellY = (index / sheetCountWidth);

	float texX = (float) cellX / sheetCountWidth;
	float texY = (float) cellY / sheetCountHeight;

	float dX = 1.0f / sheetCountWidth;
	float dY = 1.0f / sheetCountHeight;

	vertices[0].texCoords = { texX, texY};
	vertices[1].texCoords = { texX + dX, texY };
	vertices[2].texCoords = { texX + dX, texY + dY };
	vertices[3].texCoords = { texX, texY + dY };

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}