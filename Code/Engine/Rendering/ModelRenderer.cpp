#include "ModelRenderer.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/RenderEngine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Editor/imgui_widgets.h"

#include "stb_image.h"

#include <Windows.h>
#include <commdlg.h>
#include <ShObjIdl.h>
#include <Engine/Debug/Debug.h>


ModelRenderer::~ModelRenderer()
{
}

void ModelRenderer::Update()
{
	//draw();
}

void ModelRenderer::OnComponentAdded()
{
	gameObject->SetRenderComponent((RenderableComponent*)this);

	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);

	decalPosition = glm::vec3(0.0f);

	for (auto it = begin(_meshes); it != end(_meshes); ++it) 
	{
		it->SetGameObject(gameObject);
	}
}

void ModelRenderer::DrawInspector()
{
	ImGui::Separator();
	ImGui::Text("MODEL RENDERER");
	ImGui::Separator();
	char modelNameText[100];
	strcpy(modelNameText, "Model name: ");
	strcat(modelNameText, _modelName.c_str());
	ImGui::Text(modelNameText);
	ImGui::Separator();

	if (ImGui::Button("Select Decal"))
	{
		std::string selectedFile = OpenFileDialog();
		if (!selectedFile.empty())
		{
			Debug::Log("Selected file: " + selectedFile);
			// You can now use the selected file path in your application
			decalTextureId = RenderEngine::GetInstance()->GetTextureID(selectedFile.c_str(), true);
			for (auto it = begin(_meshes); it != end(_meshes); ++it)
			{
				it->setDecalTexture(decalTextureId);
			}
		}
	}

	if (decalTextureId != 0)
	{
		ImGui::Text("Decal Position");
		nimgui::draw_vec3_widget("Decal Position", decalPosition);
		ImGui::Text("Decal Target");
		nimgui::draw_vec3_widget("Decal Target", decalTarget);
		ImGui::Text("Decal Offset");
		nimgui::draw_vec3_widget("Decal Offset", decalOffset);
		for (auto it = begin(_meshes); it != end(_meshes); ++it)
		{
			it->setDecalPosition(decalPosition);
			it->setDecalTarget(decalTarget);
			it->setDecalOffset(decalOffset);
		}
		ImGui::Image((void*)(intptr_t)decalTextureId, ImVec2(256, 256));
	}
	
}

void ModelRenderer::draw(Camera* camera)
{
	for (auto it = begin(_meshes); it != end(_meshes); ++it) 
	{
		it->DrawLightingPass(camera);
	}
}

void ModelRenderer::drawOutline(Camera* camera)
{
	for (auto it = begin(_meshes); it != end(_meshes); ++it)
	{
		it->DrawOutlinePass(camera);
	}
}

void ModelRenderer::setPosition(glm::vec3 _position)
{
	this->position = _position;
}

void ModelRenderer::DrawShadows()
{
	for (auto it = begin(_meshes); it != end(_meshes); ++it)
	{
		it->DrawShadowPass();
	}
}

void ModelRenderer::DrawCubemapShadows()
{
	for (auto it = begin(_meshes); it != end(_meshes); ++it)
	{
		it->DrawCubemapShadowPass();
	}
}

void ModelRenderer::DrawPickingColor(int index)
{
	for (auto it = begin(_meshes); it != end(_meshes); ++it)
	{
		it->DrawPickingColorPass(index);
	}
}

void ModelRenderer::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
}

void ModelRenderer::setRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
}


void ModelRenderer::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void ModelRenderer::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		_meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

MeshRenderer ModelRenderer::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	MeshRenderer* _mesh = new MeshRenderer(vertices, indices);
	//_mesh->setTexture(RenderEngine::GetInstance()->GetTextureID("Concrete.jpg"));
	if (textures.size() > 0)
		_mesh->setTexture(textures[0].id);
	else
		_mesh->setTexture(RenderEngine::GetInstance()->GetTextureID("Concrete.jpg"));
	_mesh->setProgram(RenderEngine::GetInstance()->GetShaderProgram());
	_mesh->setShadowsProgram(RenderEngine::GetInstance()->GetShadowShaderProgram());
	_mesh->setCubeMapShadowsProgram(RenderEngine::GetInstance()->GetCubeMapShadowShaderProgram());
	_mesh->setOutlineProgram(RenderEngine::GetInstance()->GetOutlineShaderProgram());
	_mesh->setPickingProgram(RenderEngine::GetInstance()->GetPickingShaderProgram());
	_mesh->shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	_mesh->shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	return *_mesh;
}

std::vector<Texture> ModelRenderer::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), directory, false);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}

unsigned int ModelRenderer::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

std::string ModelRenderer::OpenFileDialog()
{
	// Configure the OPENFILENAME structure
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;  // If using SDL, you may need to pass the correct window handle
	ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	std::string filePath;

	// Open the file dialog and get the result
	if (GetOpenFileName(&ofn))
	{
		filePath = fileName;
	}

	return filePath;
}

glm::vec3 ModelRenderer::GetPickingColor(int i)
{
	return glm::vec3(1.0, 0.0, 0.0);
}
