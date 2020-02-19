#include "RootSignature.h"

RootSignature::RootSignature(ID3D12Device5* device5)
{
	if(!this->CreateRootSignature(device5))
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Error: Failed to create RootSignature!");
	}
}

RootSignature::~RootSignature()
{

}

ID3D12RootSignature* RootSignature::GetRootSig()
{
	return this->rootSig;
}

bool RootSignature::CreateRootSignature(ID3D12Device5* device5)
{
	bool rootSignatureCreated = true;

	D3D12_ROOT_PARAMETER rootParam[1]{};

	rootParam[RS::POSITION].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
	rootParam[RS::POSITION].Descriptor.ShaderRegister = 0;
	rootParam[RS::POSITION].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rsDesc;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;	// We dont use input layout... 
	rsDesc.NumParameters = ARRAYSIZE(rootParam);
	rsDesc.pParameters = rootParam;
	rsDesc.NumStaticSamplers = 0; // TODO: Create Static Sampler

	// HUR MAN SKA SAMPLA FRÅN TEXTUR?
	// D3D12_STATIC_SAMPLER_DESC ssd{};
	// ssd.ShaderRegister = 0;
	// ssd.Filter = D3D12_FILTER_ANISOTROPIC;
	// ssd.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	// ssd.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	// ssd.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	// ssd.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	// ssd.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// rsDesc.pStaticSamplers = &ssd;	// Vad händer här?

	ID3DBlob* sBlob;

	HRESULT result = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&sBlob,
		nullptr);

	if (result != S_OK)
	{
		rootSignatureCreated = false;
	}

	result = device5->CreateRootSignature(
		0,
		sBlob->GetBufferPointer(),
		sBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSig));

	if (result != S_OK)
	{
		rootSignatureCreated = false;
	}

	return rootSignatureCreated;
}
