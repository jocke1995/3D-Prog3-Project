#include "ThreadPool.h"

ThreadPool::ThreadPool(int nrOfThreads)
{
	// Create Mutex (with no initial owner)
	this->mutex = CreateMutex(NULL, false, NULL);

	// Create Threads
	for (int i = 0; i < nrOfThreads; i++)
		this->threads.push_back(new Thread(&this->taskQueue, &this->mutex));
}

ThreadPool::~ThreadPool()
{
	for (Thread* thread : this->threads)
		delete thread;
}

void ThreadPool::WaitForThreads()
{
	// two conditios to wait.
	// 1. Until the taskQueue is empty
	// 2. Until each thread's taskpointer is nullptr
	// Otherwise there is a chance that the mainthread will continue
	// whilst some threads are working on some of the last tasks.

	// Wait until all tasks are completed
	while (true)
		if (this->IsAllFinished() && this->taskQueue.empty())
			break;
}

void ThreadPool::AddTask(Task* task)
{
	// ------------------- Critical region -------------------
	// Lock
	WaitForSingleObject(this->mutex, INFINITE);

	this->taskQueue.push(task);

	// Unlock
	ReleaseMutex(this->mutex);
	// ------------------ - Critical region -------------------
}

bool ThreadPool::IsAllFinished()
{
	for (Thread* thread : this->threads)
		if (thread->IsTaskNullptr() == false)
			return false;

	return true;
}
