#include "ThreadPool.h"

ThreadPool::ThreadPool(int nrOfThreads)
{
	this->nrOfThreads = nrOfThreads;
	// Create Mutex (with no initial owner)
	//this->mutex = CreateMutex(NULL, false, NULL);
}

ThreadPool::~ThreadPool()
{
	for (Thread* thread : this->threads)
		delete thread;

	//CloseHandle(this->mutex);
}

void ThreadPool::CreateThreads()
{
	// Create Threads
	for (int i = 0; i < this->nrOfThreads; i++)
		this->threads.push_back(new Thread(&this->taskQueue, &this->mutex));
}

void ThreadPool::WaitForThreads()
{
	// two conditios to wait.
	// 1. Until the taskQueue is empty
	// 2. Until each thread's taskpointer is nullptr
	// Otherwise there is a chance that the mainthread will continue
	// whilst some threads are working on some of the last tasks.

	// Wait until all tasks are completed
	bool taskEmpty = false;
	while (true)
	{
		this->mutex.lock();
		//WaitForSingleObject(this->mutex, INFINITE);
		taskEmpty = this->taskQueue.empty();
		//ReleaseMutex(this->mutex);
		this->mutex.unlock();
		
		if (taskEmpty && this->IsAllFinished())
		{
			break;
		}
	}
}

void ThreadPool::AddTask(Task* task)
{
	// ------------------- Critical region -------------------
	//WaitForSingleObject(this->mutex, INFINITE);
	this->mutex.lock();

	this->taskQueue.push(task);

	this->mutex.unlock();
	//ReleaseMutex(this->mutex);
	// ------------------- Critical region -------------------
}

void ThreadPool::ExitThreads()
{
	for (auto thread : this->threads)
	{
		thread->Running(false);
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
