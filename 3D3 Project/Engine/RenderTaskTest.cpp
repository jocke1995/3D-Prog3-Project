#include "RenderTaskTest.h"

RenderTaskTest::RenderTaskTest()
{
	this->GetPipelineState()->CreateShader(L"VertexShader.hlsl", ShaderType::VS);
	this->GetPipelineState()->CreateShader(L"PixelShader.hlsl", ShaderType::PS);

	this->SPECIFY_GRAPHICS_PIPELINE_STATE_DESC();
}

RenderTaskTest::~RenderTaskTest()
{
}

void RenderTaskTest::SPECIFY_GRAPHICS_PIPELINE_STATE_DESC()
{
	this->Getgpsd()->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	this->Getgpsd()->VS.pShaderBytecode = this->GetPipelineState()->GetShader(ShaderType::VS)->GetBlob()->GetBufferPointer();
	this->Getgpsd()->VS.BytecodeLength = this->GetPipelineState()->GetShader(ShaderType::VS)->GetBlob()->GetBufferSize();
	this->Getgpsd()->PS.pShaderBytecode = this->GetPipelineState()->GetShader(ShaderType::PS)->GetBlob()->GetBufferPointer();
	this->Getgpsd()->PS.BytecodeLength = this->GetPipelineState()->GetShader(ShaderType::PS)->GetBlob()->GetBufferSize();

	// RenderTarget
	this->Getgpsd()->RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	this->Getgpsd()->NumRenderTargets = 1;
	// Depthstencil usage
	this->Getgpsd()->SampleDesc.Count = 1;
	this->Getgpsd()->SampleMask = UINT_MAX;
	// Rasterizer behaviour
	this->Getgpsd()->RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	this->Getgpsd()->RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// Specify Blend descriptions
	D3D12_RENDER_TARGET_BLEND_DESC defaultRTdesc = {
		false, false,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL };
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		this->Getgpsd()->BlendState.RenderTarget[i] = defaultRTdesc;
}
