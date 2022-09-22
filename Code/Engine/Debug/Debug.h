#pragma once
#include <string>
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

private:
	static void Color(int color);
};

