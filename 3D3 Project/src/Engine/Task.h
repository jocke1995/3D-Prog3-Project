#pragma once

#include <Windows.h>
#include <process.h>	// _beginThreadex

class Task
{
public:
	Task();
	~Task();

	virtual void Execute() = 0;
};

