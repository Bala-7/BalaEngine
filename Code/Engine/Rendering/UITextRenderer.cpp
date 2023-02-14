#include "UITextRenderer.h"
#include "Engine/Rendering/RenderEngine.h"

UITextRenderer::UITextRenderer()
{
	componentType = ComponentType::UITEXT_RENDERER;
	program = RenderEngine::GetInstance()->GetTextShaderProgram();
	projection = glm::ortho(0.0f, static_cast<GLfloat>(1280), 0.0f, static_cast<GLfloat>(720));
	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void UITextRenderer::Update()
{
	draw();
}

void UITextRenderer::OnComponentAdded()
{
	if(debug)
		RenderEngine::GetInstance()->AddTextToDebugUI(this);
	else
		RenderEngine::GetInstance()->AddTextToUI(this);
}

void UITextRenderer::setPosition(glm::vec2 _position)
{
	position = _position;
}

void UITextRenderer::setScale(float _scale)
{
	scale = _scale;
}

void UITextRenderer::setColor(glm::vec3 _color)
{
	color = _color;
}

void UITextRenderer::setProgram(GLuint _program)
{
	program = _program;
}

void UITextRenderer::setText(std::string _text)
{
	text = _text;
}

void UITextRenderer::setDebug(bool _debug)
{
	debug = _debug;
}

void UITextRenderer::draw()
{
	// activate corresponding render state	
	glUseProgram(this->program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	int x = position.x;
	int y = position.y;

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		RenderEngine::Character ch = RenderEngine::GetInstance()->GetCharacterList()[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6)* scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void UITextRenderer::DrawInspector()
{
}