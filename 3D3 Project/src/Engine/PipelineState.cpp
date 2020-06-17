#include "PipelineState.h"

PipelineState::PipelineState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsd)
{
	// Set the rootSignature in the pipeline state object descriptor
	this->gpsd = *gpsd;

	this->gpsd.pRootSignature = rootSignature->GetRootSig();

	this->CreateShader(VSName, ShaderType::VS);
	this->CreateShader(PSName, ShaderType::PS);

	ID3DBlob* vsBlob = this->VS->GetBlob();
	ID3DBlob* psBlob = this->PS->GetBlob();

	this->gpsd.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	this->gpsd.VS.BytecodeLength = vsBlob->GetBufferSize();
	this->gpsd.PS.pShaderBytecode = psBlob->GetBufferPointer();
	this->gpsd.PS.BytecodeLength = psBlob->GetBufferSize();

	// Create pipelineStateObject
	HRESULT hr = device->CreateGraphicsPipelineState(&this->gpsd, IID_PPV_ARGS(&this->PSO));

	this->PSO->SetName(L"GraphicsPSO");
	if (!SUCCEEDED(hr))
	{
		OutputDebugStringW(L"Failed to create GraphicsPSO");
	}
}

PipelineState::PipelineState(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR CSName)
{
	this->cpsd.pRootSignature = rootSignature->GetRootSig();

	this->CreateShader(CSName, ShaderType::CS);

	ID3DBlob* csBlob = this->CS->GetBlob();

	this->cpsd.CS.pShaderBytecode = csBlob->GetBufferPointer();
	this->cpsd.CS.BytecodeLength = csBlob->GetBufferSize();

	// Create pipelineStateObject
	HRESULT hr = device->CreateComputePipelineState(&this->cpsd, IID_PPV_ARGS(&this->PSO));

	this->PSO->SetName(L"ComputePSO");
	if (!SUCCEEDED(hr))
	{
		OutputDebugStringW(L"Failed to create ComputePSO");
	}
}

PipelineState::~PipelineState()
{
	SAFE_RELEASE(&this->PSO);
}

ID3D12PipelineState* PipelineState::GetPSO() const
{
	return this->PSO;
}

const D3D12_GRAPHICS_PIPELINE_STATE_DESC* PipelineState::GetGpsd() const
{
	return &this->gpsd;
}

const D3D12_COMPUTE_PIPELINE_STATE_DESC* PipelineState::GetCpsd()  const
{
	return &this->cpsd;
}

Shader* PipelineState::GetShader(ShaderType type) const
{
	if (type == ShaderType::VS)
	{
		return this->VS;
	}
	else if (type == ShaderType::PS)
	{
		return this->PS;
	}
	else if (type == ShaderType::CS)
	{
		return this->CS;
	}

	return nullptr;
}

void PipelineState::CreateShader(LPCTSTR fileName, ShaderType type)
{
	if (type == ShaderType::VS)
	{
		this->VS = AssetLoader::Get()->LoadShader(fileName, type);
	}
	else if (type == ShaderType::PS)
	{
		this->PS = AssetLoader::Get()->LoadShader(fileName, type);
	}
	else if (type == ShaderType::CS)
	{
		this->CS = AssetLoader::Get()->LoadShader(fileName, type);
	}
}
