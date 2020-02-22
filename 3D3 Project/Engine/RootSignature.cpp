#include "RootSignature.h"

RootSignature::RootSignature()
{
	if(!this->CreateRootSignatureStructure())
	{
		OutputDebugStringA("Error: Something went wrong when creating RootSignature\n");
	}
}

RootSignature::~RootSignature()
{

}

ID3D12RootSignature** RootSignature::GetRootSig()
{
	return &this->rootSig;
}

ID3DBlob* RootSignature::GetBlob()
{
	return this->sBlob;
}

bool RootSignature::CreateRootSignatureStructure()
{
	D3D12_ROOT_PARAMETER rootParam[1]{};

	rootParam[RS::POSITION].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParam[RS::POSITION].Descriptor.ShaderRegister = 0;
	rootParam[RS::POSITION].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

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
		return false;
	}
	return true;
}
