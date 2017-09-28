#pragma once
#include <chrono>
#include <iostream>
#include <string>

#define TIMER_NANO(x) {Debug::startTimer(); x; Debug::endTimerNano(#x);}
#define TIMER_MICRO(x) {Debug::startTimer(); x; Debug::endTimerMicro(#x);}
#define TIMER_MILLI(x) {Debug::startTimer(); x; Debug::endTimerMilli(#x);}

enum LogType
{
	Info,
	Warning,
	Error
};

class Debug
{
public:
	static void startTimer();
	static void endTimerNano(std::string line);
	static void endTimerMicro(std::string line);
	static void endTimerMilli(std::string line);

	static void Log(std::string msg, LogType type = Info);

private:
	static std::chrono::high_resolution_clock::time_point time;
};

