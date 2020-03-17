#include "RenderTaskBlend.h"

RenderTaskBlend::RenderTaskBlend(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds)
{
	
}

RenderTaskBlend::~RenderTaskBlend()
{
}

extern D3D12::D3D12Timer timer;
void RenderTaskBlend::Execute(int backBufferIndex)
{
	this->commandAllocators[backBufferIndex]->Reset();
	this->commandLists[backBufferIndex]->Reset(this->commandAllocators[backBufferIndex], NULL);

	// Start timestamp
	UINT timer_index = 1;
	timer.start(this->commandLists[backBufferIndex], timer_index);

	this->commandLists[backBufferIndex]->SetGraphicsRootSignature(this->rootSig);
	
	ID3D12DescriptorHeap* bindlessHeap = this->descriptorHeap->GetID3D12DescriptorHeap();
	this->commandLists[backBufferIndex]->SetDescriptorHeaps(1, &bindlessHeap);

	this->commandLists[backBufferIndex]->SetGraphicsRootDescriptorTable(RS::dtSRV, this->descriptorHeap->GetGPUHeapAt(0));

	// Change state on front/backbuffer
	this->commandLists[backBufferIndex]->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	DescriptorHeap* renderTargetHeap = this->renderTargets[0]->GetDescriptorHeap();
	DescriptorHeap* depthBufferHeap = this->depthBuffer->GetDescriptorHeap();

	
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetHeap->GetCPUHeapAt(backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsh = depthBufferHeap->GetCPUHeapAt(0);

	this->commandLists[backBufferIndex]->OMSetRenderTargets(1, &cdh, true, &dsh);


	D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	this->commandLists[backBufferIndex]->RSSetViewports(1, viewPort);
	this->commandLists[backBufferIndex]->RSSetScissorRects(1, rect);
	this->commandLists[backBufferIndex]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw
	XMFLOAT4X4* viewProjMat = this->camera->GetViewProjMatrix();
	XMMATRIX tmpViewProjMat = XMLoadFloat4x4(viewProjMat);

	for (auto object : this->objects)
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

		this->commandLists[backBufferIndex]->SetGraphicsRoot32BitConstants(RS::CB_PER_OBJECT_CONSTANTS, sizeof(CB_PER_OBJECT) / sizeof(UINT), &perObject, 0);

		// Draw each object twice with different PSO 
		for (int i = 0; i < 2; i++)
		{
			this->commandLists[backBufferIndex]->SetPipelineState(this->pipelineStates[i]->GetPSO());
			this->commandLists[backBufferIndex]->DrawInstanced(num_vertices, 1, 0, 0);
		}
	}
	
	// Ändra state på front/backbuffer
	this->commandLists[backBufferIndex]->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	// End timestamp
	timer.stop(this->commandLists[backBufferIndex], timer_index);
	timer.resolveQueryToCPU(this->commandLists[backBufferIndex], timer_index);

	this->commandLists[backBufferIndex]->Close();
}
