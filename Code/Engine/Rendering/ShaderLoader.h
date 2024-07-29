#pragma once

#include <GL/glew.h>
#include <string>

class ShaderLoader {
public:
	GLuint createProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename);
	GLuint createProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename);

private:
	std::string readShader(const char* filename);
	GLuint createShader(GLenum shaderType, std::string source, const char* shaderName);
};

