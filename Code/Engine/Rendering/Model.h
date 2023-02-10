#pragma once
#include "Engine/Rendering/Mesh.h"
#include "Engine/Rendering/Shader.h"
#include <vector>
#include <string>
#include <assimp/scene.h>

#include <GL/glew.h>

class Model
{
public:
	Model(const char* path)
	{
		loadModel(path);
	}
	void Draw(Shader& shader);

private:
	// model data
	std::vector<Mesh> meshes;
	std::string directory;
	GLuint program;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

