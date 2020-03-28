#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include "Shader.h"
#include "RootSignature.h"
#include "../AssetLoader.h"

class PipelineState
{
public:
	PipelineState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsd);
	PipelineState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName);
	~PipelineState();

	ID3D12PipelineState* GetPSO();
	D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetGpsd();
	D3D12_COMPUTE_PIPELINE_STATE_DESC* GetCpsd();

	Shader* GetShader(ShaderType type);
private:
	LPCTSTR name;
	ID3D12PipelineState* PSO = nullptr;

	// For Graphics
	Shader* VS = nullptr;
	Shader* PS = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};

	// For Compute
	Shader* CS = nullptr;
	D3D12_COMPUTE_PIPELINE_STATE_DESC cpsd = {};

	void CreateShader(LPCTSTR fileName, ShaderType type);
};

#endif