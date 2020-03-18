#pragma once

#include <vector>
#include "Thread.h"

class ThreadPool
{
public:
	ThreadPool(int nrOfThreads = 2);
	~ThreadPool();
	void CreateThreads();

	void WaitForThreads();

	void AddTask(Task* task);

	void ExitThreads();
private:
	int nrOfThreads;
	std::vector<Thread*> threads;
	std::queue<Task*> taskQueue;

	// Mutex for synchronization whilst pushing/popping the taskQueue
	//HANDLE mutex;
	std::mutex mutex;
	bool IsAllFinished();
};

