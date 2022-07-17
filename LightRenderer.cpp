#include "LightRenderer.h"


LightRenderer::LightRenderer(MeshType meshType, Camera* camera) 
{
	this->camera = camera;

	switch (meshType)
	{
		case kTriangle: Mesh::setTriData(vertices, indices); break;
		case kQuad: Mesh::setQuadData(vertices, indices); break;
		case kCube: Mesh::setCubeData(vertices, indices); break;
		case kSphere: Mesh::setSphereData(vertices, indices); break;
		default: break;
	}

	// Generate and bind VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate VBO
	glGenBuffers(1, &vbo);
	// Specify buffer type
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass the data we want to store in the buffer
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(Vertex) * vertices.size(),
		&vertices[0],
		GL_STATIC_DRAW);

	// Enable vertex attribute number 0 (position)
	glEnableVertexAttribArray(0);
	// Set the attribute that we want to use
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Enable one more attribute pointer to color the object
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::color)));

	// Generate and bind EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void LightRenderer::draw() 
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(glm::mat4(1.0), position);

	// Use the shader program
	glUseProgram(this->program);

	// Set the value of model, view and projection matrices
	GLint modelLocation = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	
	glm::mat4 view = camera->getViewMatrix();
	GLint viewLocation = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	
	glm::mat4 projectionMatrix = camera->getProjectionMatrix();
	GLint projectionLocation = glGetUniformLocation(program, "projection"); 
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// For safety, unbind vertex array and program variable by setting their values to 0
	glBindVertexArray(0);
	glUseProgram(0);
}

LightRenderer::~LightRenderer() 
{
}

void LightRenderer::setPosition(glm::vec3 _position) 
{
	position = _position;
}

void LightRenderer::setColor(glm::vec3 _color) 
{
	this->color = _color;
}

void LightRenderer::setProgram(GLuint _program) 
{
	this->program = _program;
}

//getters
glm::vec3 LightRenderer::getPosition() 
{
	return position;
}

glm::vec3 LightRenderer::getColor() 
{
	return color;
}