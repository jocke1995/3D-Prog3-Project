#ifndef COMPUTESTATE_H
#define COMPUTESTATE_H

#include "PipelineState.h"
class ComputeState : public PipelineState
{
public:
	ComputeState(	ID3D12Device5* device, 
					RootSignature* rootSignature, 
					LPCWSTR CSName,		// name of the shader
					LPCTSTR psoName);	// name of the PSO

	virtual ~ComputeState();

	const D3D12_COMPUTE_PIPELINE_STATE_DESC* GetCpsd() const;
	Shader* GetShader(ShaderType type = ShaderType::CS) const;
private:
	// For Compute
	Shader* CS = nullptr;
	D3D12_COMPUTE_PIPELINE_STATE_DESC cpsd = {};
};

#endif 