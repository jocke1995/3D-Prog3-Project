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

void RenderTaskTest::Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex)
{
	commandAllocator->Reset();
	commandList5->Reset(commandAllocator, NULL);

	commandList5->SetGraphicsRootSignature(rootSig);

	// Change state on front/backbuffer
	commandList5->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		*this->renderTargets[0]->GetRenderTarget(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	DescriptorHeap* dHeap = this->renderTargets[0]->GetDescriptorHeap();
	
	D3D12_CPU_DESCRIPTOR_HANDLE* cdh = dHeap->GetCPUHeapAt(backBufferIndex);

	commandList5->OMSetRenderTargets(1, cdh, true, NULL);

	float clearColor[] = { 0.0f, 0.1f, 0.1f, 1.0f };
	commandList5->ClearRenderTargetView(*cdh, clearColor, 0, nullptr);

	D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	commandList5->RSSetViewports(1, viewPort);
	commandList5->RSSetScissorRects(1, rect);
	commandList5->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList5->SetPipelineState(*this->pipelineState->GetPSO());

	// Draw
	size_t num_vertices = 0;
	ID3D12Resource1* vbResource;
	Transform* transform;
	for (auto object : this->objects)
	{
		transform = object->GetTransform();
		vbResource = *object->GetMesh()->GetVBResource();

		// TODO: Change when we have setup the rootsignature correctly
		commandList5->SetGraphicsRootShaderResourceView(RS::POSITION,
			vbResource->GetGPUVirtualAddress());

		commandList5->SetGraphicsRootConstantBufferView(RS::TRANSFORM,
			transform->GetGPUAddress());

		num_vertices = object->GetMesh()->GetNumVertices();
		commandList5->DrawInstanced(num_vertices, 1, 0, 0);
	}
	

	// Ändra state på front/backbuffer
	commandList5->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		*this->renderTargets[0]->GetRenderTarget(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	commandList5->Close();
}

void RenderTaskTest::SPECIFY_GRAPHICS_PIPELINE_STATE_DESC()
{
	this->GetGpsd()->PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	ID3DBlob* blobVS = *this->GetPipelineState()->GetShader(ShaderType::VS)->GetBlob();
	ID3DBlob* blobPS = *this->GetPipelineState()->GetShader(ShaderType::PS)->GetBlob();

	this->GetGpsd()->VS.pShaderBytecode = blobVS->GetBufferPointer();
	this->GetGpsd()->VS.BytecodeLength = blobVS->GetBufferSize();
	this->GetGpsd()->PS.pShaderBytecode = blobPS->GetBufferPointer();
	this->GetGpsd()->PS.BytecodeLength = blobPS->GetBufferSize();

	// RenderTarget
	this->GetGpsd()->RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	this->GetGpsd()->NumRenderTargets = 1;
	// Depthstencil usage
	this->GetGpsd()->SampleDesc.Count = 1;
	this->GetGpsd()->SampleMask = UINT_MAX;
	// Rasterizer behaviour
	this->GetGpsd()->RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	this->GetGpsd()->RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// Specify Blend descriptions
	D3D12_RENDER_TARGET_BLEND_DESC defaultRTdesc = {
		false, false,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP, D3D12_COLOR_WRITE_ENABLE_ALL };
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		this->GetGpsd()->BlendState.RenderTarget[i] = defaultRTdesc;
}
