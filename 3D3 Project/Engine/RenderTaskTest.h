#ifndef RENDERTASKTEST_H
#define RENDERTASKTEST_H

#include "RenderTask.h"


class RenderTaskTest : public RenderTask
{
public:
	RenderTaskTest();
	~RenderTaskTest();

private:
	void SPECIFY_GRAPHICS_PIPELINE_STATE_DESC();
};

#endif