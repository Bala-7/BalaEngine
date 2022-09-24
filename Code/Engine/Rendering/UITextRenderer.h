#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <string>
#include "Engine/Core/Component.h"

class UITextRenderer : public Component
{
public:
	UITextRenderer();

	void Update() override;
	void OnComponentAdded() override;

	void setPosition(glm::vec2 _position);
	void setScale(float _scale);
	void setColor(glm::vec3 _color);
	void setProgram(GLuint _program);
	void setText(std::string _text);
	void setDebug(bool _debug);

private:
	void draw();

	std::string text;
	glm::mat4 projection;
	GLuint VAO, VBO, ebo, texture, program;
	glm::vec2 position;
	float scale;
	glm::vec3 color;
	bool debug = false;
};

