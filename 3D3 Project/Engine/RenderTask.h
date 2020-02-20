#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "PipelineState.h"
//#include "Object.h"
#include "RenderTarget.h"


class RenderTask
{
public:
	RenderTask();
	~RenderTask();


private:

	PipelineState* pipelineState = nullptr;



};

#endif