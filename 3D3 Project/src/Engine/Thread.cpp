#include "Thread.h"
#include "Task.h"

unsigned int __stdcall Thread::threadFunc(LPVOID lpParameter)
{
	Thread* threadInstance = (Thread*)lpParameter;

	while (threadInstance->isRunning)
	{
		// ------------------- Critical region -------------------
		threadInstance->mutex.lock();

		if (!threadInstance->taskQueue.empty())
		{
			// Get a task from the queue
			threadInstance->task = threadInstance->taskQueue.front();
			// Remove the task from the queue
			threadInstance->taskQueue.pop();
		}

		threadInstance->mutex.unlock();
		// ------------------ - Critical region -------------------

		// Safetycheck if the thread has a task assigned
		if (threadInstance->task != nullptr)
		{
			threadInstance->task->Execute();
			threadInstance->task = nullptr;
		}
	}
	return 0;
}

Thread::Thread()
	: mutex()
{
	this->thread = (HANDLE)_beginthreadex(0, 0, this->threadFunc, this, 0, 0);
}

Thread::~Thread()
{
	CloseHandle(this->thread);
}

bool Thread::IsTaskNullptr()
{
	if (this->task == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Thread::ExitThread()
{
	this->isRunning = false;
}

void Thread::AddTask(Task* task)
{
	this->mutex.lock();
	this->taskQueue.push(task);
	this->mutex.unlock();
}

bool Thread::IsQueueEmpty()
{
	return this->taskQueue.empty();
}
