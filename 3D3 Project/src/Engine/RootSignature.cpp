#include "RootSignature.h"

RootSignature::RootSignature(ID3D12Device5* device)
{
	this->CreateRootSignatureStructure();

	HRESULT hr = device->CreateRootSignature(
		0,
		this->sBlob->GetBufferPointer(),
		this->sBlob->GetBufferSize(),
		IID_PPV_ARGS(&this->rootSig));

	if(FAILED(hr))
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
	D3D12_DESCRIPTOR_RANGE dtRangesSRV[1]{};
	dtRangesSRV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	dtRangesSRV[0].NumDescriptors = -1; // Bindless
	dtRangesSRV[0].BaseShaderRegister = 0;
	dtRangesSRV[0].RegisterSpace = 0; // space0

	D3D12_ROOT_DESCRIPTOR_TABLE dtSRV = {};
	dtSRV.NumDescriptorRanges = ARRAYSIZE(dtRangesSRV);
	dtSRV.pDescriptorRanges = dtRangesSRV;

	D3D12_ROOT_PARAMETER rootParam[RS::NUM_PARAMS]{};

	rootParam[RS::dtSRV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[RS::dtSRV].DescriptorTable = dtSRV;
	rootParam[RS::dtSRV].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS::CB_PER_OBJECT_CONSTANTS].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].Constants.ShaderRegister = 0; // b0
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].Constants.RegisterSpace = 0; // space0
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].Constants.Num32BitValues = sizeof(CB_PER_OBJECT) / sizeof(UINT);
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS::ColorCBV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[RS::ColorCBV].Descriptor.ShaderRegister = 1; // b0
	rootParam[RS::ColorCBV].Descriptor.RegisterSpace = 0; // space0
	rootParam[RS::ColorCBV].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS::ColorUAV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
	rootParam[RS::ColorUAV].Descriptor.ShaderRegister = 0; // c0
	rootParam[RS::ColorUAV].Descriptor.RegisterSpace = 0; // space0
	rootParam[RS::ColorUAV].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rsDesc;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;	// We dont use input layout... 
	rsDesc.NumParameters = ARRAYSIZE(rootParam);
	rsDesc.pParameters = rootParam;
	rsDesc.NumStaticSamplers = 0;

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
