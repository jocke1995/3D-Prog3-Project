#include "BlendRenderTask.h"

BlendRenderTask::BlendRenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, COMMAND_INTERFACE_TYPE interfaceType)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, interfaceType)
{
	
}

BlendRenderTask::~BlendRenderTask()
{
}

void BlendRenderTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);

	commandList->SetGraphicsRootSignature(this->rootSig);
	
	ID3D12DescriptorHeap* bindlessHeap = this->descriptorHeap->GetID3D12DescriptorHeap();
	commandList->SetDescriptorHeaps(1, &bindlessHeap);

	commandList->SetGraphicsRootDescriptorTable(RS::dtSRV, this->descriptorHeap->GetGPUHeapAt(0));

	// Change state on front/backbuffer
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetResource(this->backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	DescriptorHeap* renderTargetHeap = this->renderTargets[0]->GetDescriptorHeap();
	DescriptorHeap* depthBufferHeap = this->depthBuffer->GetDescriptorHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetHeap->GetCPUHeapAt(this->backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsh = depthBufferHeap->GetCPUHeapAt(0);

	commandList->OMSetRenderTargets(1, &cdh, true, &dsh);

	const D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	const D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	commandList->RSSetViewports(1, viewPort);
	commandList->RSSetScissorRects(1, rect);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const XMFLOAT4X4* const viewProjMat = this->camera->GetViewProjMatrix();
	XMMATRIX tmpViewProjMat = XMLoadFloat4x4(viewProjMat);

	// Draw from opposite order from the sorted array
	for(int i = this->entities.size() - 1; i >= 0; i--)
	{
		// Get the renderComponent of the entity
		RenderComponent* rc = this->entities.at(i)->GetComponent<RenderComponent>();

		// Check if the Entity is to be drawn in Blend
		if (rc->GetDrawFlag() & DrawOptions::Blend)
		{
			// Draw for every mesh the entity has
			for (unsigned int j = 0; j < rc->GetNrOfMeshes(); j++)
			{
				size_t num_vertices = rc->GetMesh(j)->GetNumVertices();
				SlotInfo* info = rc->GetSlotInfo(j);

				Transform* transform = rc->GetTransform();
				const XMFLOAT4X4* worldMat = transform->GetWorldMatrix();
				XMFLOAT4X4 WVPTransposed;
				XMFLOAT4X4 wTransposed;

				XMMATRIX tmpWorldMat = XMLoadFloat4x4(worldMat);
				XMMATRIX tmpWVP = tmpWorldMat * tmpViewProjMat;

				// Store and transpose the matrices for shader
				XMStoreFloat4x4(&WVPTransposed, XMMatrixTranspose(tmpWVP));
				XMStoreFloat4x4(&wTransposed, XMMatrixTranspose(tmpWorldMat));



				// Create a CB_PER_ENTITY struct
				CB_PER_ENTITY perEntity = { wTransposed, WVPTransposed, *info };

				commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_ENTITY_CONSTANTS, sizeof(CB_PER_ENTITY) / sizeof(UINT), &perEntity, 0);

				// Draw each entity twice with different PSO 
				for (int k = 0; k < 2; k++)
				{
					commandList->SetPipelineState(this->pipelineStates[k]->GetPSO());
					commandList->DrawInstanced(num_vertices, 1, 0, 0);
				}
			}
		}	
	}
	
	// Ändra state på front/backbuffer
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetResource(this->backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	commandList->Close();
}
