#ifndef COMPONENT_H
#define COMPONENT_H

// TODO: precompiled header problem
#include "../../Engine/stdafx.h"

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Update() = 0;

private:
};

#endif
