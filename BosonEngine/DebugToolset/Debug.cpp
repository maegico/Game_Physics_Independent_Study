#include "Debug.h"

std::chrono::high_resolution_clock::time_point Debug::time;

void Debug::startTimer()
{
	time = std::chrono::high_resolution_clock::now();
}

void Debug::endTimerNano(std::string line)
{
	auto originalTime = time;
	time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time - originalTime).count();

	printf("Line: %s\nTotal Time Required: %lld nanoseconds.\n", line.c_str(), duration);
}

void Debug::endTimerMicro(std::string line)
{
	auto originalTime = time;
	time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time - originalTime).count();

	printf("Line: %s\nTotal Time Required: %lld microseconds.\n", line.c_str(), duration);
}

void Debug::endTimerMilli(std::string line)
{
	auto originalTime = time;
	time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time - originalTime).count();

	printf("Line: %s\nTotal Time Required: %lld milliseconds.\n", line.c_str(), duration);
}

void Debug::Log(std::string msg, LogType type)
{
	switch (type)
	{
	case Warning:
		printf("WARNING: %s\n", msg.c_str());
		break;
	case Error:
		printf("ERROR: %s\n", msg.c_str());
		break;
	default:
		printf("INFO: %s\n", msg.c_str());
		break;
	}
}
