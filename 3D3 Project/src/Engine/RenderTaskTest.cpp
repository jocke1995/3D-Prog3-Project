#include "RenderTaskTest.h"
#include <stdlib.h>
RenderTaskTest::RenderTaskTest(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_QUEUE_TYPE cqType)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, cqType)
{
	
}

RenderTaskTest::~RenderTaskTest()
{
}

extern D3D12::D3D12Timer timer;
void RenderTaskTest::Execute()
{
	this->commandAllocators[this->backBufferIndex]->Reset();
	this->commandLists[this->backBufferIndex]->Reset(this->commandAllocators[this->backBufferIndex], NULL);

	// Start timestamp
	UINT timer_index = 0;
	timer.start(this->commandLists[this->backBufferIndex], timer_index);

	this->commandLists[this->backBufferIndex]->SetGraphicsRootSignature(this->rootSig);
	
	ID3D12DescriptorHeap* bindlessHeap = this->descriptorHeap->GetID3D12DescriptorHeap();
	this->commandLists[this->backBufferIndex]->SetDescriptorHeaps(1, &bindlessHeap);

	this->commandLists[this->backBufferIndex]->SetGraphicsRootDescriptorTable(RS::dtSRV, this->descriptorHeap->GetGPUHeapAt(0));

	// Change state on front/backbuffer
	this->commandLists[this->backBufferIndex]->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(this->backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	DescriptorHeap* renderTargetHeap = this->renderTargets[0]->GetDescriptorHeap();
	DescriptorHeap* depthBufferHeap = this->depthBuffer->GetDescriptorHeap();

	
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetHeap->GetCPUHeapAt(this->backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsh = depthBufferHeap->GetCPUHeapAt(0);

	this->commandLists[this->backBufferIndex]->OMSetRenderTargets(1, &cdh, true, &dsh);

	static float r = 0.0f;
	static float g = 0.0f;
	static float b = 0.0f;


	// For fun blinking background
	//r += 0.0005f;
	//g += 0.005f;
	//b += 0.2f;
	//
	//float testr = abs(sinf(r));
	//float testg = abs(sinf(g));
	//float testb = abs(sinf(b));

	float clearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	this->commandLists[this->backBufferIndex]->ClearRenderTargetView(cdh, clearColor, 0, nullptr);
	this->commandLists[this->backBufferIndex]->ClearDepthStencilView(dsh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	this->commandLists[this->backBufferIndex]->RSSetViewports(1, viewPort);
	this->commandLists[this->backBufferIndex]->RSSetScissorRects(1, rect);
	this->commandLists[this->backBufferIndex]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->commandLists[this->backBufferIndex]->SetPipelineState(this->pipelineStates[0]->GetPSO());

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

		this->commandLists[this->backBufferIndex]->SetGraphicsRoot32BitConstants(RS::CB_PER_OBJECT_CONSTANTS, sizeof(CB_PER_OBJECT) / sizeof(UINT), &perObject, 0);

		this->commandLists[this->backBufferIndex]->DrawInstanced(num_vertices, 1, 0, 0);
	}
	

	// Ändra state på front/backbuffer
	this->commandLists[this->backBufferIndex]->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(this->backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	// End timestamp
	timer.stop(this->commandLists[this->backBufferIndex], timer_index);
	timer.resolveQueryToCPU(this->commandLists[this->backBufferIndex], timer_index);

	this->commandLists[this->backBufferIndex]->Close();
}
