#pragma once

#include "Task.h"
#include <queue>
#include <mutex>

#include <Windows.h>
#include <process.h>	// _beginThreadex

class Thread
{
public:
	Thread(std::queue<Task*> *taskQueue, std::mutex* mutex);
	~Thread();

	bool IsTaskNullptr();
private:
	HANDLE thread;

	static unsigned int __stdcall threadFunc(LPVOID lpParameter);

	std::queue<Task*> *taskQueue;

	Task* task= nullptr;
	//HANDLE* mutex = nullptr;
	std::mutex* mutex = nullptr;
};

