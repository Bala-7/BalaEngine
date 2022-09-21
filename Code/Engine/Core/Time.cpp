#include "Time.h"

std::chrono::milliseconds Time::frameStartMilliseconds;
std::chrono::milliseconds Time::frameEndMilliseconds;
std::chrono::milliseconds Time::deltaTimeMilliseconds;

void Time::StartFrame()
{
	frameStartMilliseconds = TimePointToMilliseconds(Clock::now());
}

void Time::EndFrame()
{
	frameEndMilliseconds = TimePointToMilliseconds(Clock::now());
	deltaTimeMilliseconds = frameEndMilliseconds - frameStartMilliseconds;
}

std::chrono::milliseconds Time::GetDeltaTime()
{
	return deltaTimeMilliseconds;
}

std::chrono::milliseconds Time::TimePointToMilliseconds(std::chrono::time_point<Clock> timePoint)
{
	return std::chrono::time_point_cast<std::chrono::milliseconds>(timePoint).time_since_epoch();
}
