#pragma once
#include <string>
#include <sstream>
#include "Engine/Rendering/UITextRenderer.h"

class Debug
{
public: 
	static void Log(const char* message);
	static void LogWarning(const char* message);
	static void LogError(const char* message);

	static void Log(std::string message);
	static void LogWarning(std::string message);
	static void LogError(std::string message);

	static const char* GetLogBuffer();
	static const char* GetWarningsBuffer();
	static const char* GetErrorBuffer();

private:
	static void Color(int color);

	static std::stringstream logBuffer;
	static std::stringstream warningsBuffer;
	static std::stringstream errorBuffer;
};

