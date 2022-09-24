#pragma once
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;

class Time
{
public:

	static std::chrono::milliseconds Now();

	static void StartLoop();
	static void EndLoop();


	static float GetDeltaTime();
	static std::chrono::milliseconds GetDeltaTimeMS();
	static std::chrono::milliseconds GetLastFrameEnd();
	static float GetLastLoopTime();
	static float GetLastFrameTime();

private:

	static std::chrono::milliseconds loopStartMilliseconds;
	static std::chrono::milliseconds loopEndMilliseconds;

	static std::chrono::milliseconds loopPrevStartMilliseconds;
	static std::chrono::milliseconds loopPrevEndMilliseconds;

	static std::chrono::milliseconds deltaTimeMilliseconds;

	static std::chrono::milliseconds TimePointToMilliseconds(std::chrono::time_point<Clock> timePoint);
};

