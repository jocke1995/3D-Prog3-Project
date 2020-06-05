#include "ForwardRenderTask.h"

FowardRenderTask::FowardRenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_INTERFACE_TYPE interfaceType)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, interfaceType)
{
	
}

FowardRenderTask::~FowardRenderTask()
{
}

//extern D3D12::D3D12Timer timer;
void FowardRenderTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);

	// Start timestamp
	//UINT timer_index = 1;
	//timer.start(commandList, timer_index);

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

	

	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	commandList->ClearRenderTargetView(cdh, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	commandList->RSSetViewports(1, viewPort);
	commandList->RSSetScissorRects(1, rect);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->SetPipelineState(this->pipelineStates[0]->GetPSO());

	// Draw
	XMFLOAT4X4* viewProjMat = this->camera->GetViewProjMatrix();
	XMMATRIX tmpViewProjMat = XMLoadFloat4x4(viewProjMat);

	for (auto entity : this->entities)
	{
		// Get the renderComponent of the entity
		RenderComponent* rc = entity->GetComponent<RenderComponent>();

		// Check if the entity is to be drawn in forwardRendering
		if (rc->GetDrawFlag() & DrawOptions::ForwardRendering)
		{
			size_t num_vertices = rc->GetMesh()->GetNumVertices();
			Transform* transform = rc->GetTransform();

			XMFLOAT4X4* worldMat = transform->GetWorldMatrix();
			XMFLOAT4X4 WVPTransposed;
			XMFLOAT4X4 wTransposed;

			XMMATRIX tmpWorldMat = XMLoadFloat4x4(worldMat);
			XMMATRIX tmpWVP = tmpWorldMat * tmpViewProjMat;

			// Store and transpose the matrices for shader
			XMStoreFloat4x4(&WVPTransposed, XMMatrixTranspose(tmpWVP));
			XMStoreFloat4x4(&wTransposed, XMMatrixTranspose(tmpWorldMat));

			SlotInfo* info = rc->GetSlotInfo();

			// Create a CB_PER_ENTITY struct
			CB_PER_ENTITY perEntity = { wTransposed, WVPTransposed, *info };

			commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_ENTITY_CONSTANTS, sizeof(CB_PER_ENTITY) / sizeof(UINT), &perEntity, 0);

			// Resource with color from the copyQueue -> Computequeue -> this DirectQueue
			commandList->SetGraphicsRootConstantBufferView(RS::ColorCBV, this->resources[0]->GetGPUVirtualAdress());

			commandList->DrawInstanced(num_vertices, 1, 0, 0);
		}
	}

	// Ändra state på front/backbuffer
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetRenderTarget(this->backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	// End timestamp
	//timer.stop(commandList, timer_index);
	//timer.resolveQueryToCPU(commandList, timer_index);

	commandList->Close();
}
