#pragma once
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

class Time
{
public:
	static void StartFrame();
	static void EndFrame();
	static std::chrono::milliseconds GetDeltaTime();

private:
	static std::chrono::milliseconds frameStartMilliseconds;
	static std::chrono::milliseconds frameEndMilliseconds;
	static std::chrono::milliseconds deltaTimeMilliseconds;

	static std::chrono::milliseconds TimePointToMilliseconds(std::chrono::time_point<Clock> timePoint);
	
};

