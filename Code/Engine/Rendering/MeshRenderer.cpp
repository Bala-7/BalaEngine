#include "MeshRenderer.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/RenderEngine.h"

MeshRenderer::MeshRenderer(MeshType modelType) 
{
	componentType = ComponentType::MESH_RENDERER;
	camera = RenderEngine::GetInstance()->GetCamera();
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	_material = new Material();

	switch (modelType) {
	case kTriangle: Mesh::setTriData(vertices, indices);
		break;
	case kQuad: Mesh::setQuadData(vertices, indices);
		break;
	case kCube: Mesh::setCubeData(vertices, indices);
		break;
	case kSphere: Mesh::setSphereData(vertices, indices);
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

MeshRenderer::~MeshRenderer() 
{
}

void MeshRenderer::Update()
{
	draw();
}

void MeshRenderer::OnComponentAdded()
{
	//RenderEngine::GetInstance()->AddSpriteToRenderList(this);
	setPosition(gameObject->transform->position);
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);
}

void MeshRenderer::draw() 
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
	modelMatrix = /*translationMatrix * */rotateMatrix * scaleMatrix;

	// vp matrix is the multiplied view and projection amtrices
	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();

	// Send the data to the shader program
	glUseProgram(this->program);

	shader->setMat4("vp", vp);
	shader->setMat4("model", modelMatrix);
	shader->setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
	shader->setVec3("lightColor", RenderEngine::GetInstance()->GetEnvironmentLight());
	
	shader->setVec3("viewPos", RenderEngine::GetInstance()->GetCamera()->getCameraPosition());
	
	shader->setVec3("material.ambient", _material->ambient);
	shader->setVec3("material.diffuse", _material->diffuse);
	shader->setVec3("material.specular", _material->specular);
	shader->setFloat("material.shininess", _material->shininess);

	shader->setVec3("light.position", glm::vec3(0.0f, 0.0f, 5.5f));
	shader->setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shader->setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
	shader->setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture);

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