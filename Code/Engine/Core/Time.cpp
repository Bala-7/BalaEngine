#include "Time.h"

float Time::lastFrameTime;
std::chrono::microseconds Time::deltaTimeMicroseconds;

std::chrono::microseconds Time::Now()
{
	return TimePointToMicroseconds(Clock::now());
}


float Time::GetDeltaTime()
{
	return deltaTimeMicroseconds.count() / 1000.0f;
}

std::chrono::microseconds Time::GetDeltaTimeMS()
{
	return deltaTimeMicroseconds;
}

float Time::GetLastFrameTime()
{
	return lastFrameTime;
}

void Time::SetLastFrameTime(float newLastFrameTime)
{
	lastFrameTime = newLastFrameTime;
}


std::chrono::microseconds Time::TimePointToMicroseconds(std::chrono::time_point<Clock> timePoint)
{
	return std::chrono::time_point_cast<std::chrono::microseconds>(timePoint).time_since_epoch();
}
