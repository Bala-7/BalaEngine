#pragma once
#include <vector>
#include "Camera.h"
#include "LightRenderer.h"
#include "Light.h"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Engine/Core/Component.h"
#include "Engine/Rendering/Shader.h"
#include "Engine/Rendering/RenderableComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Model.h"


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
	MeshRenderer(std::vector<Vertex> _vertices, std::vector<GLuint> _indices);

	~MeshRenderer();

	void Update() override;
	void OnComponentAdded() override;
	void DrawInspector() override;

	void DrawShadowPass();
	void DrawCubemapShadowPass();
	void DrawPickingColorPass(int index);
	void DrawLightingPass(Camera* camera);
	void DrawOutlinePass(Camera* camera);
	void draw();
	void SetupShaderForLightingPass(Camera* camera);
	void SetupShaderForOutlinePass(Camera* camera);
	void SetupShaderFor2DShadowPass();
	void SetupShaderForPickingColorPass(int index);
	void SetupShaderForCubeMapShadowPass();
	void setPosition(glm::vec3 _position);
	void setScale(glm::vec3 _scale);
	void setRotation(glm::vec3 _rotation);
	void setProgram(GLuint _program);
	void setShadowsProgram(GLuint _program);
	void setCubeMapShadowsProgram(GLuint _program);
	void setPickingProgram(GLuint _program);
	void setOutlineProgram(GLuint _program);
	void setTexture(GLuint _textureID);
	void setDecalTexture(GLuint _textureID);
	void setDecalPosition(glm::vec3 position);
	void setDecalTarget(glm::vec3 target);
	void setDecalOffset(glm::vec3 offset);
	void setLight(Light light);
	void setModel(std::string path);

	void setTriangles(std::vector<Vertex> _vertices, std::vector<GLuint> _indices);

	Material* GetMaterial();

	Shader* shader;
	Shader* shadowShader;
	Shader* cubeMapShadowShader;
	Shader* pickingShader;
	Shader* outlineShader;
	
	// TEMP : Shadow mapping
	static glm::mat4 lightViewProjectionMatrix;
	// \TEMP\

private:
	Light* _light;
	Material* _material;
	std::vector<Vertex>vertices;
	std::vector<GLuint>indices;
	glm::mat4 modelMatrix;
	Camera* renderCamera;
	glm::vec3 position, scale, rotation;
	GLuint vao, vbo, ebo, texture, program, shadowsProgram, cubeMapShadowsProgram, decalTexture, pickingProgram, outlineShaderProgram;
	glm::vec3 decalPosition, decalTarget, decalOffset;

	// Model loading
	std::string directory;
	std::vector<Mesh> meshes;
	std::string modelPath;

	
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

	// Object picking
	glm::vec3 GetPickingColor(int index);

};

