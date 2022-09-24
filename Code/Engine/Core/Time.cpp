#include "Time.h"

std::chrono::milliseconds Time::loopStartMilliseconds;
std::chrono::milliseconds Time::loopEndMilliseconds;

std::chrono::milliseconds Time::loopPrevStartMilliseconds;
std::chrono::milliseconds Time::loopPrevEndMilliseconds;


std::chrono::milliseconds Time::deltaTimeMilliseconds;

std::chrono::milliseconds Time::Now()
{
	return TimePointToMilliseconds(Clock::now());
}


void Time::StartLoop()
{
	loopPrevStartMilliseconds = loopStartMilliseconds;
	loopStartMilliseconds = TimePointToMilliseconds(Clock::now());
}

void Time::EndLoop()
{
	loopPrevEndMilliseconds = loopEndMilliseconds;
	loopEndMilliseconds = TimePointToMilliseconds(Clock::now());
	deltaTimeMilliseconds = loopEndMilliseconds - loopStartMilliseconds;
}


float Time::GetDeltaTime()
{
	return deltaTimeMilliseconds.count() / 1000.0f;
}

std::chrono::milliseconds Time::GetDeltaTimeMS()
{
	return deltaTimeMilliseconds;
}


float Time::GetLastLoopTime()
{
	return loopEndMilliseconds.count() - loopPrevStartMilliseconds.count();
}


std::chrono::milliseconds Time::TimePointToMilliseconds(std::chrono::time_point<Clock> timePoint)
{
	return std::chrono::time_point_cast<std::chrono::milliseconds>(timePoint).time_since_epoch();
}
