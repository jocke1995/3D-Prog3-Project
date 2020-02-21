#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include "stdafx.h"
#include "Shader.h"

class PipelineState
{
public:
	PipelineState();
	~PipelineState();

	void CreateShader(LPCTSTR fileName, ShaderType type);
	
	ID3D12PipelineState** GetPSO();
	Shader* GetShader(ShaderType type);
private:
	LPCTSTR name;
	ID3D12PipelineState* PSO = nullptr;
	Shader* VS = nullptr;
	Shader* PS = nullptr;

};

#endif