#include "RenderTaskTest.h"

RenderTaskTest::RenderTaskTest(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsdTest)
	:RenderTask(device, rootSignature, VSName, PSName, gpsdTest)
{
	
}

RenderTaskTest::~RenderTaskTest()
{
}

void RenderTaskTest::Execute(ID3D12CommandAllocator* commandAllocator, ID3D12GraphicsCommandList5* commandList5, ID3D12RootSignature* rootSig, int backBufferIndex)
{
	commandAllocator->Reset();
	commandList5->Reset(commandAllocator, NULL);

	commandList5->SetGraphicsRootSignature(rootSig);
	
	auto a = this->descriptorHeap->GetID3D12DescriptorHeap();
	commandList5->SetDescriptorHeaps(1, &a);

	commandList5->SetGraphicsRootDescriptorTable(RS::dtSRV, this->descriptorHeap->GetGPUHeapAt(0));

	// Change state on front/backbuffer
	commandList5->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	DescriptorHeap* renderTargetHeap = this->renderTargets[0]->GetDescriptorHeap();
	DescriptorHeap* depthBufferHeap = this->depthBuffer->GetDescriptorHeap();

	
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetHeap->GetCPUHeapAt(backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsh = depthBufferHeap->GetCPUHeapAt(0);

	commandList5->OMSetRenderTargets(1, &cdh, true, &dsh);

	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	commandList5->ClearRenderTargetView(cdh, clearColor, 0, nullptr);
	commandList5->ClearDepthStencilView(dsh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	commandList5->RSSetViewports(1, viewPort);
	commandList5->RSSetScissorRects(1, rect);
	commandList5->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList5->SetPipelineState(this->pipelineState->GetPSO());

	// Draw
	
	

	XMFLOAT4X4* viewProjMat = this->camera->GetViewProjMatrix();
	XMMATRIX tmpViewProjMat = XMLoadFloat4x4(viewProjMat);

	for (auto object : *this->objects)
	{
		size_t num_vertices = object->GetMesh()->GetNumVertices();
		Transform* transform = object->GetTransform();

		XMFLOAT4X4* worldMat = transform->GetWorldMatrix();
		XMFLOAT4X4 WVPTransposed;
		XMFLOAT4X4 wTransposed;

		XMMATRIX tmpWorldMat = XMLoadFloat4x4(worldMat);
		XMMATRIX tmpWVP = tmpWorldMat * tmpViewProjMat;

		// Store and transpose the matrices for shader
		XMStoreFloat4x4(&WVPTransposed, XMMatrixTranspose(tmpWVP));
		XMStoreFloat4x4(&wTransposed, XMMatrixTranspose(tmpWorldMat));

		SlotInfo* info = object->GetSlotInfo();

		// Create a CB_PER_OBJECT struct
		CB_PER_OBJECT perObject = { wTransposed, WVPTransposed, *info };

		commandList5->SetGraphicsRoot32BitConstants(RS::CB_PER_OBJECT_CONSTANTS, sizeof(CB_PER_OBJECT) / sizeof(UINT), &perObject, 0);

		commandList5->DrawInstanced(num_vertices, 1, 0, 0);
	}
	

	// Ändra state på front/backbuffer
	commandList5->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	commandList5->Close();
}
