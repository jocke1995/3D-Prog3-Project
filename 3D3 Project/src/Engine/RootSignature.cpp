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
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to create RootSignature\n");
	}
}

RootSignature::~RootSignature()
{
	SAFE_RELEASE(&this->rootSig);
	SAFE_RELEASE(&this->sBlob);
}

ID3D12RootSignature* RootSignature::GetRootSig() const
{
	return this->rootSig;
}

ID3DBlob* RootSignature::GetBlob() const
{
	return this->sBlob;
}

void RootSignature::CreateRootSignatureStructure()
{
	// DescriptorTable for CBV's (bindless)
	D3D12_DESCRIPTOR_RANGE dtRangesCBV[3]{};
	dtRangesCBV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	dtRangesCBV[0].NumDescriptors = -1;
	dtRangesCBV[0].BaseShaderRegister = 0;	// b0
	dtRangesCBV[0].RegisterSpace = 0;		// space0
	dtRangesCBV[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	dtRangesCBV[1].NumDescriptors = -1;
	dtRangesCBV[1].BaseShaderRegister = 0;	// b0
	dtRangesCBV[1].RegisterSpace = 1;		// space1
	dtRangesCBV[2].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	dtRangesCBV[2].NumDescriptors = -1;
	dtRangesCBV[2].BaseShaderRegister = 0;	// b0
	dtRangesCBV[2].RegisterSpace = 2;		// space2
	D3D12_ROOT_DESCRIPTOR_TABLE dtCBV = {};
	dtCBV.NumDescriptorRanges = ARRAYSIZE(dtRangesCBV);
	dtCBV.pDescriptorRanges = dtRangesCBV;

	// DescriptorTable for SRV's (bindless)
	D3D12_DESCRIPTOR_RANGE dtRangesSRV[1]{};
	dtRangesSRV[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	dtRangesSRV[0].NumDescriptors = -1; // Bindless
	dtRangesSRV[0].BaseShaderRegister = 0;	// t0
	dtRangesSRV[0].RegisterSpace = 0;

	D3D12_ROOT_DESCRIPTOR_TABLE dtSRV = {};
	dtSRV.NumDescriptorRanges = ARRAYSIZE(dtRangesSRV);
	dtSRV.pDescriptorRanges = dtRangesSRV;

	D3D12_ROOT_PARAMETER rootParam[RS::NUM_PARAMS]{};

	rootParam[RS::dtCBV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[RS::dtCBV].DescriptorTable = dtCBV;
	rootParam[RS::dtCBV].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	rootParam[RS::dtSRV].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParam[RS::dtSRV].DescriptorTable = dtSRV;
	rootParam[RS::dtSRV].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS::CB_PER_OBJECT_CONSTANTS].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].Constants.ShaderRegister = 1; // b1
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].Constants.RegisterSpace = 3; // space3
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].Constants.Num32BitValues = sizeof(CB_PER_OBJECT_STRUCT) / sizeof(UINT);
	rootParam[RS::CB_PER_OBJECT_CONSTANTS].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS::CB_PER_FRAME].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[RS::CB_PER_FRAME].Constants.ShaderRegister = 2; // b2
	rootParam[RS::CB_PER_FRAME].Constants.RegisterSpace = 3; // space3
	rootParam[RS::CB_PER_FRAME].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParam[RS::CB_PER_SCENE].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParam[RS::CB_PER_SCENE].Descriptor.ShaderRegister = 3;	// b3
	rootParam[RS::CB_PER_SCENE].Descriptor.RegisterSpace = 3;	// space3
	rootParam[RS::CB_PER_SCENE].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_DESC rsDesc;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;	// We dont use input layout... 
	rsDesc.NumParameters = ARRAYSIZE(rootParam);
	rsDesc.pParameters = rootParam;
	rsDesc.NumStaticSamplers = 1;

	D3D12_STATIC_SAMPLER_DESC ssd{};
	ssd.ShaderRegister = 0;
	ssd.Filter = D3D12_FILTER_ANISOTROPIC;
	ssd.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	ssd.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	ssd.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	ssd.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	ssd.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	rsDesc.pStaticSamplers = &ssd;

	ID3DBlob* errorMessages = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&this->sBlob,
		&errorMessages);

	if (hr != S_OK)
	{
		Log::PrintSeverity(Log::Severity::CRITICAL, "Failed to Serialize RootSignature\n");
	}

	if (FAILED(hr) && errorMessages)
	{
		const char* errorMsg = (const char*)errorMessages->GetBufferPointer();

		Log::PrintSeverity(Log::Severity::CRITICAL, "%s\n", errorMsg);
	}
}
