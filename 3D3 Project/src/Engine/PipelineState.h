#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include "stdafx.h"
#include "Shader.h"
#include "RootSignature.h"
#include "../AssetLoader.h"

class PipelineState
{
public:
	PipelineState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsdTest);
	~PipelineState();

	ID3D12PipelineState* GetPSO();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetGpsd();

	Shader* GetShader(ShaderType type);
private:
	LPCTSTR name;
	ID3D12PipelineState* PSO = nullptr;
	Shader* VS = nullptr;
	Shader* PS = nullptr;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};

	void CreateShader(LPCTSTR fileName, ShaderType type);
};

#endif