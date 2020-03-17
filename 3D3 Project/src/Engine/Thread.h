#pragma once

#include "Task.h"
#include <queue>

class Thread
{
public:
	Thread(std::queue<Task*> *taskQueue, HANDLE* mutex);
	~Thread();

	bool IsTaskNullptr();
private:
	HANDLE thread;

	static unsigned int __stdcall threadFunc(LPVOID lpParameter);

	std::queue<Task*> *taskQueue;

	Task* task= nullptr;
	HANDLE* mutex = nullptr;
};

