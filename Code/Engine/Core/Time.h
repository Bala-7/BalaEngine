#pragma once
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

class Time
{
public:

	static std::chrono::microseconds Now();

	static float GetDeltaTime();
	static std::chrono::microseconds GetDeltaTimeMS();

	static float GetLastFrameTime();
	static void SetLastFrameTime(float newLastFrameTime);

private:

	static float lastFrameTime;
	static std::chrono::microseconds deltaTimeMicroseconds;

	static std::chrono::microseconds TimePointToMicroseconds(std::chrono::time_point<Clock> timePoint);
};

