#pragma once

#include <iostream>
#include <vector>

#include "Thread.h"
class ThreadPool
{
public:
	ThreadPool(int nrOfThreads);
	~ThreadPool();

	void WaitForThreads();

	void AddTask(Task* task);
private:
	std::vector<Thread*> threads;
	std::queue<Task*> taskQueue;

	// Mutex for synchronization whilst pushing/popping the taskQueue
	HANDLE mutex;

	bool IsAllFinished();
};

