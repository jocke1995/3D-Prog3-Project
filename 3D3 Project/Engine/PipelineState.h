#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include "stdafx.h"
#include "Shader.h"

class PipelineState
{
public:
	PipelineState();
	~PipelineState();


private:
	LPCTSTR name;
	ID3D12PipelineState* PSO = nullptr;
	Shader* VS = nullptr;
	Shader* PS = nullptr;

};

#endif