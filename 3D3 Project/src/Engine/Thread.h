#pragma once

#include "Task.h"

enum THREAD_FLAG
{
	COPY = BIT(1),
	COMPUTE = BIT(2),
	DIRECT = BIT(3),
	ALL = BIT(4)
	// etc..
};

class Thread
{
public:
	Thread();
	~Thread();

	bool IsTaskNullptr();

	void AddTask(Task* task, unsigned int taskFlag);
	void ExitThread();

	bool IsQueueEmpty();
	unsigned int GetTaskFlag();
private:
	HANDLE thread;
	HANDLE beginEvent;

	static unsigned int __stdcall threadFunc(LPVOID lpParameter);

	std::queue<Task*> taskQueue;
	std::mutex mutex;

	Task* task= nullptr;
	unsigned int taskFlag = 0;

	bool isRunning = true;
};

