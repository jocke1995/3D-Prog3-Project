#ifndef PIPELINESTATE_H
#define PIPELINESTATE_H

#include "Shader.h"
#include "RootSignature.h"
#include "AssetLoader.h"

class PipelineState
{
public:
	PipelineState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsd);
	PipelineState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName);
	~PipelineState();

	ID3D12PipelineState* GetPSO() const;
	const D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetGpsd() const;
	const D3D12_COMPUTE_PIPELINE_STATE_DESC*  GetCpsd() const;
	Shader* GetShader(ShaderType type) const;

private:
	LPCTSTR name;
	ID3D12PipelineState* PSO = nullptr;

	void CreateShader(LPCTSTR fileName, ShaderType type);

	// For Graphics
	Shader* VS = nullptr;
	Shader* PS = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};

	// For Compute
	Shader* CS = nullptr;
	D3D12_COMPUTE_PIPELINE_STATE_DESC cpsd = {};

};

#endif