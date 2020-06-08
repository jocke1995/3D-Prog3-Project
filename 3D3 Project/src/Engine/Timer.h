#ifndef TIMER_H
#define TIMER_H

#include "../Window.h"

class Timer
{
public:
	Timer(Window* window);
	~Timer();

	void Update();
	double GetDeltaTime();
private:
	Window* window;

	double dt = 0.0f;

	std::chrono::time_point<std::chrono::system_clock> timeNow;
	std::chrono::time_point<std::chrono::system_clock> timeLast;

	std::chrono::time_point<std::chrono::system_clock> timeLastTitleUpdate;
};

#endif