#include "Timer.h"

Timer::Timer(Window* window)
{
	this->window = window;

	auto start = std::chrono::system_clock::now();
	this->timeNow = start;
	this->timeLast = start;
}

Timer::~Timer()
{
}

void Timer::Update()
{
    // Calculate deltatime
    this->timeLast = this->timeNow;
    this->timeNow = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = this->timeNow - this->timeLast;
    this->dt = elapsed_time.count();

    // Set limit to the updates on the window title
    std::chrono::duration<double> elapsed_timeFps = this->timeNow - this->timeLastTitleUpdate;
    if (elapsed_timeFps.count() >= 1.0)
    {
        std::string fpsString = std::to_string(int(1.0 / this->dt));
        std::wstring tmp = std::wstring(fpsString.begin(), fpsString.end());
        window->SetWindowTitle(tmp);
        this->timeLastTitleUpdate = this->timeNow;
    }
}

double Timer::GetDeltaTime() const
{
    return this->dt;
}

void Timer::StartTimer()
{
    this->start = std::chrono::system_clock::now();
}

double Timer::StopTimer()
{
    this->stop = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsedTime = this->stop - this->start;

    return elapsedTime.count();
}
