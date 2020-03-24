#pragma once

#include "Thread.h"

class ThreadPool
{
public:
	ThreadPool(int nrOfThreads);
	~ThreadPool();

	void WaitForThreads();

	void AddTask(Task* task);

	void ExitThreads();
private:
	std::vector<Thread*> threads;

	int nrOfThreads;
	unsigned int threadCounter = 0;

	bool IsAllFinished();
	bool IsThreadsQueuesEmpty();
};

