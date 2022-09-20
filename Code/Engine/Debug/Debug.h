#pragma once


class Debug
{
public: 
	static void Log(const char* message);
	static void LogWarning(const char* message);
	static void LogError(const char* message);

private:
	static void Color(int color);
};

