#include "ComputeState.h"

ComputeState::ComputeState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName, LPCTSTR psoName)
	:PipelineState(psoName)
{
	this->cpsd.pRootSignature = rootSignature->GetRootSig();

	this->CS = this->CreateShader(CSName, ShaderType::CS);

	ID3DBlob* csBlob = this->CS->GetBlob();

	this->cpsd.CS.pShaderBytecode = csBlob->GetBufferPointer();
	this->cpsd.CS.BytecodeLength = csBlob->GetBufferSize();

	// Create pipelineStateObject
	HRESULT hr = device->CreateComputePipelineState(&this->cpsd, IID_PPV_ARGS(&this->PSO));

	this->PSO->SetName(this->psoName);
	if (!SUCCEEDED(hr))
	{
		// Log failed to create "psoName"
		OutputDebugStringW(L"Failed to create ComputePSO");
	}
}

ComputeState::~ComputeState()
{
}

const D3D12_COMPUTE_PIPELINE_STATE_DESC* ComputeState::GetCpsd() const
{
	return &this->cpsd;
}

Shader* ComputeState::GetShader(ShaderType type) const
{
	if (type == ShaderType::CS)
	{
		return this->CS;
	}

	// LOG no shader of that type in this pso
	return nullptr;
}
