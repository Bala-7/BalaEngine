#include "Engine/Rendering/SkyboxRenderer.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/RenderEngine.h"


SkyboxRenderer::SkyboxRenderer()
{
	//componentType = ComponentType::SKYBOX_RENDERER;
	renderCamera = RenderEngine::GetInstance()->GetCamera();

	// Generate and bind VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Set attributes: position(0), normal(1) and texture coords(2)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::texCoords)));

	// Unbind Buffers and vertexArray
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void SkyboxRenderer::Update()
{
	//setupShaderForDraw();
	//draw();
}

void SkyboxRenderer::setPosition(glm::vec3 _position)
{
	this->position = _position;
}

void SkyboxRenderer::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
}

void SkyboxRenderer::setRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
}

void SkyboxRenderer::setProgram(GLuint _program)
{
	this->program = _program;
	shader = new Shader(_program);
}

void SkyboxRenderer::setShaderProgram(GLuint _program)
{
	this->program = _program;
	shader = new Shader(program);
}

void SkyboxRenderer::setTexture(GLuint textureID)
{
	texture = textureID;
}

void SkyboxRenderer::DrawInspector()
{
}

void SkyboxRenderer::OnComponentAdded()
{
	gameObject->SetRenderComponent((RenderableComponent*)this);
}

void SkyboxRenderer::DrawShadows()
{
}

void SkyboxRenderer::DrawCubemapShadows()
{
}

void SkyboxRenderer::setVerticesData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	std::vector<Vertex> _vertices = {
		{ // Front face
			{ -0.5f, -0.5f, -0.5f }, // Position
			{ 0.0f, 0.0f, -1.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 0.0, 0.0 } // Texture Coordinate
		}, // 0
		{
			{ 0.5f, -0.5f, -0.5f },
			{ 0.0f, 0.0f, -1.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 1.0, 0.0 }
		}, // 1
		{
			{ 0.5f, 0.5f, -0.5f },
			{ 0.0f, 0.0f, -1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 1.0 }
		}, // 2
		{
			{ -0.5f, 0.5f, -0.5f },
			{ 0.0f, 0.0f, -1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0, 1.0 }
		}, // 3
		{ // Back face
			{ -0.5f, -0.5f, 0.5f }, // Position
			{ 0.0f, 0.0f, 1.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 0.0, 0.0 } // Texture Coordinate
		}, // 4
		{
			{ 0.5f, -0.5f, 0.5f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 1.0, 0.0 }
		}, // 5
		{
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 1.0 }
		}, // 6
		{
			{ -0.5f, 0.5f, 0.5f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0, 1.0 }
		}, // 7
		{
			{ -0.5f, -0.5f, -0.5f }, // Position
			{ -1.0f, 0.0f, 0.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 1.0, 0.0 } // Texture Coordinate
		}, // 8
		{
			{ 0.5f, -0.5f, -0.5f },
			{ 1.0f, 0.0f, 0.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0, 0.0 }
		}, // 9
		{
			{ 0.5f, 0.5f, -0.5f },
			{ 1.0f, 0.0f, 0.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0, 1.0 }
		}, // 10
		{
			{ -0.5f, 0.5f, -0.5f },
			{ -1.0f, 0.0f, 0.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 1.0 }
		}, // 11
		{
			{ -0.5f, -0.5f, 0.5f }, // Position
			{ -1.0f, 0.0f, 0.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 0.0, 0.0 } // Texture Coordinate
		}, // 12
		{
			{ 0.5f, -0.5f, 0.5f },
			{ 1.0f, 0.0f, 0.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 1.0, 0.0 }
		}, // 13
		{
			{ 0.5f, 0.5f, 0.5f },
			{ 1.0f, 0.0f, 0.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 1.0 }
		}, // 14
		{
			{ -0.5f, 0.5f, 0.5f },
			{ -1.0f, 0.0f, 0.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0, 1.0 }
		}, // 15
		{
			{ -0.5f, -0.5f, -0.5f }, // Position
			{ 0.0f, -1.0f, 0.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 1.0, 0.0 } // Texture Coordinate
		}, // 16
		{
			{ 0.5f, -0.5f, -0.5f },
			{ 0.0f, -1.0f, 0.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 1.0, 1.0 }
		}, // 17
		{
			{ 0.5f, 0.5f, -0.5f },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 0.0 }
		}, // 18
		{
			{ -0.5f, 0.5f, -0.5f },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0, 0.0 }
		}, // 19
		{
			{ -0.5f, -0.5f, 0.5f }, // Position
			{ 0.0f, -1.0f, 0.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 0.0, 0.0 } // Texture Coordinate
		}, // 20
		{
			{ 0.5f, -0.5f, 0.5f },
			{ 0.0f, -1.0f, 0.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0, 1.0 }
		}, // 21
		{
			{ 0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 1.0 }
		}, // 22
		{
			{ -0.5f, 0.5f, 0.5f },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0, 1.0 }
		}, // 23
	};
	std::vector<uint32_t> _indices =
	{
		0, 1, 2, 0, 2, 3, // Front
		5, 4, 7, 5, 7, 6, // Back
		9, 13, 14, 9, 14, 10, // Right
		12, 8, 11, 12, 11, 15, // Left
		19, 18, 22, 19, 22, 23, // Top
		20, 21, 17, 20, 17, 16, // Bottom
	};

	vertices.clear(); indices.clear();

	vertices = _vertices;
	indices = _indices;

}

void SkyboxRenderer::setupShaderForDraw()
{
	glDepthMask(GL_FALSE);
	// Get position and scale from Transform component
	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);

	// Set the model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotateMatrix = glm::rotate(translationMatrix, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = translationMatrix * rotateMatrix * scaleMatrix;

	glm::mat4 vp = renderCamera->getProjectionMatrix() * renderCamera->getViewMatrix();

	glUseProgram(shader->ID);

	shader->setMat4("vp", vp);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	shader->setInt("skyboxTexture", 3);
}

void SkyboxRenderer::draw()
{
	// Bind the VAO and draw object
	glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Unbind VertexArray for safety
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}

void SkyboxRenderer::draw(Camera* camera)
{
	setupShaderForDraw();
	draw();
}
