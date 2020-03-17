#include "Thread.h"
#include "Task.h"

unsigned int __stdcall Thread::threadFunc(LPVOID lpParameter)
{
	Thread* threadInstance = (Thread*)lpParameter;

	while (true)
	{
		// ------------------- Critical region -------------------
		// Lock
		WaitForSingleObject(threadInstance->mutex, INFINITE);

		if (!threadInstance->taskQueue->empty())
		{
			// Get a task from the queue
			threadInstance->task = threadInstance->taskQueue->front();
			// Remove the task from the queue
			threadInstance->taskQueue->pop();
		}

		// Unlock
		ReleaseMutex(threadInstance->mutex);
		// ------------------ - Critical region -------------------

		// Check if the thread has a task assigned
		if (threadInstance->task != nullptr)
		{
			threadInstance->task->Execute();
			threadInstance->task = nullptr;
		}
	}
	return 0;
}

Thread::Thread(std::queue<Task*>* taskQueue, HANDLE* mutex)
{
	this->taskQueue = taskQueue;

	this->mutex = mutex;
	
	this->thread = (HANDLE)_beginthreadex(0, 0, this->threadFunc, this, 0, 0);
}

Thread::~Thread()
{
	CloseHandle(this->thread);
}

bool Thread::IsTaskNullptr()
{
	if (this->task == nullptr)
		return true;
	else
		return false;
}
