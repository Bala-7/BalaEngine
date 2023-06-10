#pragma once
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "Engine/Rendering/Shader.h"

enum MeshType {
	kTriangle = 0,
	kQuad = 1,
	kCube = 2,
	kSphere = 3,
	kModel = 4
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	static void setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
	static void setModelData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);
	
	
	// mesh data
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture>      textures;

private:
	//  render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

