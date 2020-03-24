#pragma once

#include "Task.h"

class Thread
{
public:
	Thread();
	~Thread();

	bool IsTaskNullptr();

	void AddTask(Task* task);
	void ExitThread();

	bool IsQueueEmpty();
private:
	HANDLE thread;

	static unsigned int __stdcall threadFunc(LPVOID lpParameter);

	std::queue<Task*> taskQueue;
	std::mutex mutex;

	Task* task= nullptr;

	bool isRunning = true;
};

