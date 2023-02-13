#include "Mesh.h"

#include <GL/glew.h>
#include "glm/glm.hpp"

void Mesh::setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
	std::vector<Vertex> _vertices = {
		{ 
			{ 0.0f, -1.0f, 0.0f }, // Position
			{ 0.0f, 0.0f, 1.0 }, // Normal
			{ 1.0f, 0.0f, 0.0 }, // Color
			{ 0.0, 1.0 } // Texture Coordinate
		}, // 0
		{ 
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 1.0f, 0.0 },
			{ 0.0, 0.0 } 
		}, // 1
		{ 
			{ -1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 0.0 } 
		}, // 2
		};
	std::vector<uint32_t> _indices = { 0, 1, 2 };

	vertices.clear(); indices.clear();
	
	vertices = _vertices;
	indices = _indices;
}

void Mesh::setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) 
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
			{ 1.0, 0.0 }
		}, // 1
		{
			{ 0.5f, 0.5f, 0.0f },
			{ 0.0f, 0.0f, 1.0 },
			{ 0.0f, 0.0f, 1.0 },
			{ 1.0, 1.0 }
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

void Mesh::setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
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

void Mesh::setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
{
	//TODO
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}