#include "RootSignature.h"

RootSignature::RootSignature(ID3D12Device5* device)
{
	this->CreateRootSignatureStructure();

	HRESULT hr = device->CreateRootSignature(
		0,
		this->sBlob->GetBufferPointer(),
		this->sBlob->GetBufferSize(),
		IID_PPV_ARGS(&this->rootSig));

	if (hr != S_OK)
	{
		OutputDebugStringA("Error: Failed to create RootSignature!\n");
	}
}

RootSignature::~RootSignature()
{
	SAFE_RELEASE(&this->rootSig);
	SAFE_RELEASE(&this->sBlob);
}

ID3D12RootSignature* RootSignature::GetRootSig()
{
	return this->rootSig;
}

ID3DBlob* RootSignature::GetBlob()
{
	return this->sBlob;
}

void RootSignature::CreateRootSignatureStructure()
{
	D3D12_ROOT_PARAMETER rootParam[RS::NUM_PARAMS]{};

	rootParam[RS::POSITION].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParam[RS::POSITION].Descriptor.ShaderRegister = 0;
	rootParam[RS::POSITION].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS::TRANSFORM].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[RS::TRANSFORM].Descriptor.ShaderRegister = 0;
	rootParam[RS::TRANSFORM].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rsDesc;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;	// We dont use input layout... 
	rsDesc.NumParameters = ARRAYSIZE(rootParam);
	rsDesc.pParameters = rootParam;
	rsDesc.NumStaticSamplers = 0; // TODO: Create Static Sampler

	// HUR MAN SKA SAMPLA FRÅN TEXTUR: 
	// D3D12_STATIC_SAMPLER_DESC ssd{};
	// ssd.ShaderRegister = 0;
	// ssd.Filter = D3D12_FILTER_ANISOTROPIC;
	// ssd.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	// ssd.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	// ssd.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	// ssd.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// ssd.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// rsDesc.pStaticSamplers = &ssd;	// Vad händer här?

	HRESULT hr = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&this->sBlob,
		nullptr);

	if (hr != S_OK)
	{
		OutputDebugStringA("Error: Failed to Serialize RootSignature!\n");
	}
}
