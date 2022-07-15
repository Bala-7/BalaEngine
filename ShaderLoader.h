#pragma once

#include <GL/glew.h>

class ShaderLoader {
public:
	GLuint CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);

private:
	std::string readShader(const char* filename);
	GLuint createShader(GLenum shaderType, std::string source, const char* shaderName);
};

