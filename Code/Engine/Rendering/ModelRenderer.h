#pragma once
#include <vector>
#include "MeshRenderer.h"

class ModelRenderer : public RenderableComponent
{
public:
	ModelRenderer(const char* path) 
	{
		loadModel(path);
		std::string pathString = path;
		_modelName = pathString.substr(pathString.find_last_of("/\\") + 1);
	}

	~ModelRenderer();

	void Update() override;
	void OnComponentAdded() override;
	void DrawInspector() override;

	void draw(Camera* camera) override;
	void drawOutline(Camera* camera) override;
	void setPosition(glm::vec3 _position);
	void DrawShadows() override;
	void DrawCubemapShadows() override;
	void DrawPickingColor(int index) override;
	void setScale(glm::vec3 _scale);
	void setRotation(glm::vec3 _rotation);

private:
	std::string _modelName;
	std::string directory;
	std::vector<MeshRenderer> _meshes;
	glm::vec3 position, scale, rotation;

	GLuint decalTextureId;
	glm::vec3 decalPosition, decalTarget, decalOffset;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	MeshRenderer processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
	std::string OpenFileDialog();

	glm::vec3 GetPickingColor(int i);
};

