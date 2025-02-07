#include "MeshRenderer.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/RenderEngine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Editor/imgui_widgets.h"

#include "stb_image.h"

glm::mat4 MeshRenderer::lightViewProjectionMatrix;

MeshRenderer::MeshRenderer(MeshType modelType) 
{
	componentType = ComponentType::MESH_RENDERER;
	renderCamera = RenderEngine::GetInstance()->GetCamera();
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	_material = new Material();
	modelPath = "Assets/Models/Samus/Samus/Samus_small.obj";

	switch (modelType) {
	case kTriangle: Mesh::setTriData(vertices, indices);
		break;
	case kQuad: Mesh::setQuadData(vertices, indices);
		break;
	case kCube: Mesh::setCubeData(vertices, indices);
		break;
	case kSphere: Mesh::setSphereData(vertices, indices);
		break;
	case kModel: loadModel(modelPath); 
		break;
	default: break;
	}

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::texCoords)));

	// Unbind Buffers and vertexArray
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

MeshRenderer::MeshRenderer(std::vector<Vertex> _vertices, std::vector<GLuint> _indices)
{
	componentType = ComponentType::MESH_RENDERER;
	renderCamera = RenderEngine::GetInstance()->GetCamera();
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	_material = new Material();
	modelPath = "Assets/Models/Samus/Samus/Samus_small.obj";

	vertices = _vertices;
	indices = _indices;

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)(offsetof(Vertex, Vertex::texCoords)));

	// Unbind Buffers and vertexArray
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MeshRenderer::setModel(std::string path)
{
	loadModel(path);
}

void MeshRenderer::setTriangles(std::vector<Vertex> _vertices, std::vector<GLuint> _indices)
{
	this->vertices = _vertices;
	this->indices = _indices;
}

MeshRenderer::~MeshRenderer() 
{
}

void MeshRenderer::Update()
{
	SetupShaderForLightingPass(renderCamera);
	draw();
}

void MeshRenderer::OnComponentAdded()
{
	//RenderEngine::GetInstance()->AddSpriteToRenderList(this);
	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);
}

void MeshRenderer::DrawShadowPass()
{
	SetupShaderFor2DShadowPass();
	draw();
}

void MeshRenderer::DrawCubemapShadowPass()
{
	SetupShaderForCubeMapShadowPass();
	draw();
}

void MeshRenderer::DrawPickingColorPass(int index) 
{
	SetupShaderForPickingColorPass(index);
	draw();
}

void MeshRenderer::SetupShaderFor2DShadowPass()
{	
	// OPTION 1: Shadow pass for directional light
	// Get position and scale from Transform component
	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);

	glUseProgram(shadowShader->ID);
	lightViewProjectionMatrix = RenderEngine::GetInstance()->GetLightViewProjectionMatrix();
	
	shadowShader->setMat4("lightvp", lightViewProjectionMatrix);

	// Set the model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotateMatrix = glm::rotate(translationMatrix, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

	//modelMatrix = glm::mat4(1.0f);
	modelMatrix = translationMatrix * rotateMatrix * scaleMatrix;
	shadowShader->setMat4("modelMatrix", modelMatrix);

	GLuint shadowMapTexture = RenderEngine::GetInstance()->GetDepthMapTexture();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
}

void MeshRenderer::SetupShaderForPickingColorPass(int index)
{
	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);

	glUseProgram(pickingShader->ID);
	glm::vec3 pickingColor = GetPickingColor(index);
	pickingShader->setVec3("pickingColor", pickingColor);
	
	// Set the VP matrix
	// vp matrix is the multiplied view and projection amtrices
	Camera* sceneViewCamera = RenderEngine::GetInstance()->GetCamera();
	glm::mat4 vp = sceneViewCamera->getProjectionMatrix() * sceneViewCamera->getViewMatrix();

	// Set the model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotateMatrix = glm::rotate(translationMatrix, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

	//modelMatrix = glm::mat4(1.0f);
	modelMatrix = translationMatrix * rotateMatrix * scaleMatrix;
	pickingShader->setMat4("model", modelMatrix);
	pickingShader->setMat4("vp", vp);

	GLuint pickingTexture = RenderEngine::GetInstance()->GetPickingTexture();
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, pickingTexture);
}

void MeshRenderer::SetupShaderForCubeMapShadowPass()
{
	// Get position and scale from Transform component
	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);

	glUseProgram(cubeMapShadowShader->ID);
	GLfloat aspect = (GLfloat)1024 / (GLfloat)1024;
	
	std::vector<Light*> sceneLights = gameObject->GetNode()->GetScene()->GetSceneLights();
	glm::vec3 lightPos = sceneLights[0]->GetGameObject()->transform->position;
	GLfloat far = sceneLights[0]->GetFarPlane();
	GLfloat near = sceneLights[0]->GetNearPlane();
	cubeMapShadowShader->setVec3("lightPos", lightPos);
	cubeMapShadowShader->setFloat("far_plane", far);

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj *	glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	for (int i = 0; i < 6; ++i)
	{
		std::string matrixIndex = "shadowMatrices[" + std::to_string(i) + "]";
		cubeMapShadowShader->setMat4(matrixIndex, shadowTransforms[i]); // We pass the mat4 to the shader here
	}

	// Set the model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotateMatrix = glm::rotate(translationMatrix, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

	modelMatrix = translationMatrix * rotateMatrix * scaleMatrix;
	cubeMapShadowShader->setMat4("modelMatrix", modelMatrix);

	GLuint shadowMapTexture = RenderEngine::GetInstance()->GetDepthCubeMapTexture();
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMapTexture);
}


void MeshRenderer::DrawLightingPass(Camera* camera)
{
	// @TODO: Move this code to an appropriate place. This is for outlining the object
	//glStencilFunc(GL_ALWAYS, 1, 0xFF); // Always pass stencil test, write reference value 1
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace stencil buffer value with reference on depth pass
	//glStencilMask(0xFF); // Enable writing to the stencil buffer
	//glDepthMask(GL_TRUE); // Enable writing to the depth buffer
	// \TODO

	SetupShaderForLightingPass(camera);
	draw();
}

void MeshRenderer::DrawOutlinePass(Camera* camera)
{
	//glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Render only where stencil value is not 1
	//glStencilMask(0x00); // Disable writing to the stencil buffer
	//glDepthMask(GL_FALSE); // Disable writing to the depth buffer

	if (this->gameObject->IsSelectedInEditor()) 
	{
		SetupShaderForOutlinePass(camera);
		draw();
	}
	

	// Restore Depth and Stencil Masks
	//glDepthMask(GL_TRUE);
	//glStencilMask(0xFF);
}
void MeshRenderer::SetupShaderForOutlinePass(Camera* camera)
{
	// Get position and scale from Transform component
	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);
	
	float outlineScale = 1.01f;
	glUseProgram(outlineShader->ID);
	glm::mat4 scaledModel = glm::scale(modelMatrix, glm::vec3(outlineScale)); // Scale up the object
	
	// vp matrix is the multiplied view and projection amtrices
	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();

	outlineShader->setVec3("cameraPos", camera->getCameraPosition());
	outlineShader->setMat4("vp", vp);
	outlineShader->setMat4("model", modelMatrix);
}

void MeshRenderer::SetupShaderForLightingPass(Camera* camera)
{
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

	// vp matrix is the multiplied view and projection amtrices
	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();

	// Send the data to the shader program
	glUseProgram(this->program);
	
	shader->setMat4("directionalLightSpaceMatrix", lightViewProjectionMatrix);
	shader->setMat4("vp", vp);
	shader->setMat4("model", modelMatrix);
	
	glm::vec3 FragPos = glm::vec3(modelMatrix * glm::vec4(position, 1.0f));
	glm::vec4 fragPosLightSpace = lightViewProjectionMatrix * glm::vec4(FragPos, 1.0f);
	glm::vec3 projCoords = glm::vec3(fragPosLightSpace.x, fragPosLightSpace.y, fragPosLightSpace.z) / fragPosLightSpace.w;

	// decal
	if (decalTexture != 0) 
	{
		//glm::vec3 decalPosition = glm::vec3(0.0, 3.0, -2.4);
		//glm::vec3 decalTarget = glm::vec3(0.0, 0.0, -1.0);
		glm::vec3 decalUp = glm::vec3(0.0, 1.0, 0.0);
		glm::mat4 decalProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);
		glm::mat4 decalView = glm::lookAt(decalPosition + decalOffset, decalTarget + decalOffset, decalUp);
		glm::mat4 decalVP = decalProjection * decalView;
		shader->setMat4("decalVP", decalVP);
		shader->setBool("enableDecal", true);
	}
	else
		shader->setBool("enableDecal", false);
	// \decal

	shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader->setVec3("environmentColor", RenderEngine::GetInstance()->GetEnvironmentLight());
	shader->setVec3("viewPos", RenderEngine::GetInstance()->GetCamera()->getCameraPosition());
	std::vector<Light*> sceneLights = gameObject->GetNode()->GetScene()->GetSceneLights();
	glm::vec3 lightPos = sceneLights[0]->GetGameObject()->transform->position;
	shader->setVec3("lightPos", lightPos);
	shader->setFloat("far_plane", sceneLights[0]->GetFarPlane());

	glActiveTexture(GL_TEXTURE0);
	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	GLuint shadowMapTexture = RenderEngine::GetInstance()->GetDepthMapTexture();
	glBindTexture(GL_TEXTURE_2D, shadowMapTexture);

	glActiveTexture(GL_TEXTURE2);
	GLuint shadowCubeMapTexture = RenderEngine::GetInstance()->GetDepthCubeMapTexture();
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMapTexture);

	if (decalTexture != 0) {
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, decalTexture);
	}

	shader->setInt("Texture", 0);
	shader->setInt("directionalLightShadowMap", 1);
	shader->setInt("pointLightShadowCubeMap", 2);
	shader->setInt("decalTexture", 3);

	shader->setVec3("material.ambient", _material->ambient);
	shader->setVec3("material.diffuse", _material->diffuse);
	shader->setVec3("material.specular", _material->specular);
	shader->setFloat("material.shininess", _material->shininess);

	

	int lightCount = sceneLights.size();
	shader->setInt("activeLights", lightCount);
	for (int i = 0; i < lightCount; ++i)
	{
		std::string lightIndex = "sceneLights[" + std::to_string(i) + "]";
		shader->setVec3(lightIndex + ".direction", sceneLights[i]->GetDirection());
		shader->setVec3(lightIndex + ".position", sceneLights[i]->GetGameObject()->transform->position);

		shader->setVec3(lightIndex + ".ambient", sceneLights[i]->GetAmbient());
		shader->setVec3(lightIndex + ".diffuse", sceneLights[i]->GetDiffuse()); // darken diffuse light a bit
		shader->setVec3(lightIndex + ".specular", sceneLights[i]->GetSpecular());

		shader->setFloat(lightIndex + ".constant", sceneLights[i]->GetConstant());
		shader->setFloat(lightIndex + ".linear", sceneLights[i]->GetLinear());
		shader->setFloat(lightIndex + ".quadratic", sceneLights[i]->GetQuadratic());

		shader->setFloat(lightIndex + ".innerCutOff", glm::cos(glm::radians(sceneLights[i]->GetInnerCutOff())));
		shader->setFloat(lightIndex + ".outerCutOff", glm::cos(glm::radians(sceneLights[i]->GetOuterCutOff())));

		shader->setBool(lightIndex + ".isDirectionalLight", sceneLights[i]->GetType() == Light::LightType::Directional);
		shader->setBool(lightIndex + ".isSpotLight", sceneLights[i]->GetType() == Light::LightType::Spot);
		shader->setBool(lightIndex + ".isPointLight", sceneLights[i]->GetType() == Light::LightType::Point);
	}
}



void MeshRenderer::draw()
{
	// Bind the VAO and draw object
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind VertexArray for safety
	glBindVertexArray(0);
}

// setters
void MeshRenderer::setTexture(GLuint textureID) 
{
	texture = textureID;
}

void MeshRenderer::setDecalTexture(GLuint textureID)
{
	decalTexture = textureID;
}

void MeshRenderer::setDecalPosition(glm::vec3 position)
{
	decalPosition = position;
}

void MeshRenderer::setDecalTarget(glm::vec3 target)
{
	decalTarget = target;
}

void MeshRenderer::setDecalOffset(glm::vec3 offset)
{
	decalOffset = offset;
}

void MeshRenderer::setLight(Light light)
{
	_light = &light;
}

Material* MeshRenderer::GetMaterial()
{
	return _material;
}

void MeshRenderer::setScale(glm::vec3 _scale) 
{
	this->scale = _scale;
}

void MeshRenderer::setPosition(glm::vec3 _position) 
{
	this->position = _position;
}

void MeshRenderer::setRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
}

void MeshRenderer::setProgram(GLuint _program) 
{
	this->program = _program;
	shader = new Shader(_program);
}

void MeshRenderer::setShadowsProgram(GLuint _program)
{
	this->shadowsProgram = _program;
	shadowShader = new Shader(_program);
}

void MeshRenderer::setCubeMapShadowsProgram(GLuint _program)
{
	this->cubeMapShadowsProgram = _program;
	cubeMapShadowShader = new Shader(_program);
}

void MeshRenderer::setPickingProgram(GLuint _program)
{
	this->pickingProgram = _program;
	pickingShader = new Shader(_program);
}

void MeshRenderer::setOutlineProgram(GLuint _program)
{
	this->outlineShaderProgram = _program;
	outlineShader = new Shader(_program);
}

// /*MODEL LOADING 

void MeshRenderer::loadModel(std::string path)
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

void MeshRenderer::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh MeshRenderer::processMesh(aiMesh* mesh, const aiScene* scene)
{
	//std::vector<Vertex> vertices;
	//std::vector<unsigned int> indices;
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

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> MeshRenderer::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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

unsigned int MeshRenderer::TextureFromFile(const char* path, const std::string& directory, bool gamma)
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

glm::vec3 MeshRenderer::GetPickingColor(int index)
{
	// Function to convert object index to a unique color
	return glm::vec3(
		((index & 0x000000FF) >> 0) / 255.0f,
		((index & 0x0000FF00) >> 8) / 255.0f,
		((index & 0x00FF0000) >> 16) / 255.0f);
}





// MODEL LOADING*/

void MeshRenderer::DrawInspector()
{
	ImGui::Separator();
	ImGui::Text("MESH RENDERER");
	ImGui::Separator();
	ImGui::Text("Ambient");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Ambient", _material->ambient);
	ImGui::Separator();
	ImGui::Text("Diffuse");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Diffuse", _material->diffuse);
	ImGui::Separator();
	ImGui::Text("Specular");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Specular", _material->specular);
	ImGui::Separator();
	ImGui::Text("Shininess");
	ImGui::Separator();
	ImGui::DragFloat("##Shininess", &_material->shininess, 0.1f, 2.0f, 256.0f, "%.2f");
}