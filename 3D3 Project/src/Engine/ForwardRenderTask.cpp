#include "ForwardRenderTask.h"

FowardRenderTask::FowardRenderTask(ID3D12Device5* device, RootSignature* rootSignature, LPCWSTR VSName, LPCWSTR PSName, std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds, LPCTSTR psoName, COMMAND_INTERFACE_TYPE interfaceType)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, psoName, interfaceType)
{
	
}

FowardRenderTask::~FowardRenderTask()
{
}

void FowardRenderTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	commandAllocator->Reset();
	commandList->Reset(commandAllocator, NULL);

	commandList->SetGraphicsRootSignature(this->rootSig);
	
	ID3D12DescriptorHeap* bindlessHeap = this->descriptorHeap_CBV_UAV_SRV->GetID3D12DescriptorHeap();
	commandList->SetDescriptorHeaps(1, &bindlessHeap);

	//commandList->SetGraphicsRootDescriptorTable(RS::dtCBV, this->descriptorHeap_CBV_UAV_SRV->GetGPUHeapAt(0));
	commandList->SetGraphicsRootDescriptorTable(RS::dtSRV, this->descriptorHeap_CBV_UAV_SRV->GetGPUHeapAt(0));

	// Change state on front/backbuffer
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetResource(this->backBufferIndex),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 0 is the swapchain (which is a renderTarget)
	DescriptorHeap* renderTargetHeap = this->renderTargets[0]->GetDescriptorHeap();
	DescriptorHeap* depthBufferHeap = this->depthBuffer->GetDescriptorHeap();

	
	D3D12_CPU_DESCRIPTOR_HANDLE cdh = renderTargetHeap->GetCPUHeapAt(this->backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsh = depthBufferHeap->GetCPUHeapAt(0);

	commandList->OMSetRenderTargets(1, &cdh, true, &dsh);

	float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	commandList->ClearRenderTargetView(cdh, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	const D3D12_VIEWPORT* viewPort = this->renderTargets[0]->GetViewPort();
	const D3D12_RECT* rect = this->renderTargets[0]->GetScissorRect();
	commandList->RSSetViewports(1, viewPort);
	commandList->RSSetScissorRects(1, rect);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	commandList->SetPipelineState(this->pipelineStates[0]->GetPSO());

	// Create a CB_PER_FRAME struct
	CB_PER_FRAME perFrame = { camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z };
	commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_FRAME_CONSTANTS, sizeof(CB_PER_FRAME) / sizeof(UINT), &perFrame, 0);

	const XMFLOAT4X4* const viewProjMat = this->camera->GetViewProjMatrix();
	XMMATRIX tmpViewProjMat = XMLoadFloat4x4(viewProjMat);

	// Draw for every entity
	for (component::RenderComponent* rc : this->renderComponents)
	{
		// Check if the entity is to be drawn in forwardRendering
		if (rc->GetDrawFlag() & DrawOptions::ForwardRendering)
		{
			// Draw for every mesh the entity has
			for (unsigned int i = 0; i < rc->GetNrOfMeshes(); i++)
			{
				size_t num_vertices = rc->GetMesh(i)->GetNumVertices();
				SlotInfo* info = rc->GetSlotInfo(i);

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
				CB_PER_OBJECT perObject = { wTransposed, WVPTransposed, *info };

				commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_OBJECT_CONSTANTS, sizeof(CB_PER_OBJECT) / sizeof(UINT), &perObject, 0);

				// Resource with color from the copyQueue -> Computequeue -> this DirectQueue
				commandList->SetGraphicsRootConstantBufferView(RS::ColorCBV, this->resources[0]->GetGPUVirtualAdress());

				commandList->DrawInstanced(num_vertices, 1, 0, 0);
			}
		}
	}

	// �ndra state p� front/backbuffer
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		this->renderTargets[0]->GetResource(this->backBufferIndex),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT));

	commandList->Close();
}
