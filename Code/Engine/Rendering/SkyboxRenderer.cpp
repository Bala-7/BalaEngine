#include "Engine/Rendering/SkyboxRenderer.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/RenderEngine.h"


SkyboxRenderer::SkyboxRenderer()
{   
	//componentType = ComponentType::SKYBOX_RENDERER;
	renderCamera = RenderEngine::GetInstance()->GetCamera();
	
	skyboxFBO = new TexturedCubeMapFBO();
	texture = skyboxFBO->LoadCubemap(faces);

	// Generate and bind VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float skyboxVertices[] = {
		// positions          
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	
	// Set attributes: position(0), normal(1) and texture coords(2)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);

	
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

void SkyboxRenderer::setupShaderForDraw()
{
	glDepthMask(GL_FALSE);
	// Get position and scale from Transform component
	//setPosition(gameObject->transform->position);
	setPosition(renderCamera->getCameraPosition());
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

	glm::vec3 FragPos = glm::vec3(modelMatrix * glm::vec4(position, 1.0f));

	glm::mat4 vp = renderCamera->getProjectionMatrix() * renderCamera->getViewMatrix();

	glUseProgram(shader->ID);

	shader->setMat4("vp", vp);
	shader->setMat4("model", modelMatrix);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	shader->setInt("skyboxTexture", 3);
}

void SkyboxRenderer::draw()
{
	glDepthFunc(GL_LEQUAL);
	// Bind the VAO and draw object
	glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Unbind VertexArray for safety
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void SkyboxRenderer::draw(Camera* camera)
{
	setupShaderForDraw();
	draw();
}