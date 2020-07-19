#ifndef COMPONENT_H
#define COMPONENT_H

// TODO: precompiled header problem
#include "../../Engine/stdafx.h"
class Entity;

class Component
{
public:
	Component(Entity* parent);
	virtual ~Component();

	virtual void Update() = 0;

protected:
	Entity* parent = nullptr;
};

#endif
