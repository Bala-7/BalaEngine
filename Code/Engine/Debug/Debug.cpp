#include "Debug.h"
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

std::stringstream Debug::logBuffer;
std::stringstream Debug::warningsBuffer;
std::stringstream Debug::errorBuffer;

void Debug::Log(const char* message)
{
	Color(7);
	std::cout << message << "\n";
	logBuffer << message << "\n";
	Color(7);
}

void Debug::LogWarning(const char* message)
{
	Color(6);
	std::cout << message << "\n";
	warningsBuffer << message << "\n";
	Color(7);
}

void Debug::LogError(const char* message)
{
	Color(4);
	std::cout << message << "\n";
	errorBuffer << message << "\n";
	Color(7);
}

void Debug::Log(std::string message)
{
	Color(7);
	std::cout << message << "\n";
	logBuffer << message << "\n";
	Color(7);
}

void Debug::LogWarning(std::string message)
{
	Color(6);
	std::cout << message << "\n";
	warningsBuffer << message << "\n";
	Color(7);
}

void Debug::LogError(std::string message)
{
	Color(4);
	std::cout << message << "\n";
	errorBuffer << message << "\n";
	Color(7);
}

const char* Debug::GetLogBuffer()
{
	return logBuffer.str().c_str();
}

const char* Debug::GetWarningsBuffer()
{
	return warningsBuffer.str().c_str();
}

const char* Debug::GetErrorBuffer()
{
	return errorBuffer.str().c_str();
}

//COLORS LIST
//1: Blue
//2: Green
//3: Cyan
//4: Red
//5: Purple
//6: Yellow (Dark)
//7: Default white
//8: Gray/Grey
//9: Bright blue
//10: Brigth green
//11: Bright cyan
//12: Bright red
//13: Pink/Magenta
//14: Yellow
//15: Bright white
//Numbers after 15 include background colors
void Debug::Color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}