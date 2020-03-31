#include "BlendRenderTask.h"

BlendRenderTask::BlendRenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_INTERFACE_TYPE interfaceType)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, interfaceType)
{
	
}

BlendRenderTask::~BlendRenderTask()
{
}

extern D3D12::D3D12Timer timer;
void BlendRenderTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);

	// Start timestamp
	UINT timer_index = 2;
	timer.start(commandList, timer_index);

	commandList->SetGraphicsRootSignature(this->rootSig);
	
	ID3D12DescriptorHeap* bindlessHeap = this->descriptorHeap->GetID3D12DescriptorHeap();
	commandList->SetDescriptorHeaps(1, &bindlessHeap);

	commandList->SetGraphicsRootDescriptorTable(RS::dtSRV, this->descriptorHeap->GetGPUHeapAt(0));

	// Change state on front/backbuffer
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(this->backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	DescriptorHeap* renderTargetHeap = this->renderTargets[0]->GetDescriptorHeap();
	DescriptorHeap* depthBufferHeap = this->depthBuffer->GetDescriptorHeap();

	
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetHeap->GetCPUHeapAt(this->backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsh = depthBufferHeap->GetCPUHeapAt(0);

	commandList->OMSetRenderTargets(1, &cdh, true, &dsh);

	D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	commandList->RSSetViewports(1, viewPort);
	commandList->RSSetScissorRects(1, rect);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

		commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_OBJECT_CONSTANTS, sizeof(CB_PER_OBJECT) / sizeof(UINT), &perObject, 0);

		// Draw each object twice with different PSO 
		for (int i = 0; i < 2; i++)
		{
			commandList->SetPipelineState(this->pipelineStates[i]->GetPSO());
			commandList->DrawInstanced(num_vertices, 1, 0, 0);
		}
	}
	
	// Ändra state på front/backbuffer
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(this->backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	// End timestamp
	timer.stop(commandList, timer_index);
	timer.resolveQueryToCPU(commandList, timer_index);

	commandList->Close();
}
