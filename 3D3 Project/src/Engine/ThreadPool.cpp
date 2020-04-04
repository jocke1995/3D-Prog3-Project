#include "ThreadPool.h"

ThreadPool::ThreadPool(int nrOfThreads)
{
	this->nrOfThreads = nrOfThreads;
	
	// Create Threads
	for (int i = 0; i < this->nrOfThreads; i++)
		this->threads.push_back(new Thread());
}

ThreadPool::~ThreadPool()
{
	for (Thread* thread : this->threads)
		delete thread;
}

void ThreadPool::WaitForThreads()
{
	// Two conditios to wait.
	// 1. Until each taskQueue of each thread is empty
	// 2. Until each thread's taskpointer is nullptr
	// Otherwise there is a chance that the mainthread will continue
	// whilst some threads are working on some of the last tasks.

	// Wait until all tasks are completed
	bool isEmpty = false;
	while (true)
	{
		isEmpty = this->IsThreadsQueuesEmpty();

		if (isEmpty && this->IsAllFinished())
		{
			break;
		}
	}
}

void ThreadPool::AddTask(Task* task)
{
	// Adds a task to a thread
	this->threads.at(this->threadCounter % this->nrOfThreads)->AddTask(task);
	this->threadCounter++;
}

void ThreadPool::ExitThreads()
{
	for (auto thread : this->threads)
	{
		thread->ExitThread();
	}
}

bool ThreadPool::IsAllFinished()
{
	for (Thread* thread : this->threads)
	{
		if (thread->IsTaskNullptr() == false)
		{
			return false;
		}
	}
	return true;
}

bool ThreadPool::IsThreadsQueuesEmpty()
{
	for (auto thread : this->threads)
	{
		if (thread->IsQueueEmpty() == false)
		{
			return false;
		}
	}
	return true;
}
