#include "BlendRenderTask.h"

BlendRenderTask::BlendRenderTask(	
	ID3D12Device5* device,
	RootSignature* rootSignature,
	LPCWSTR VSName, LPCWSTR PSName,
	std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds,
	LPCTSTR psoName)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, psoName)
{
	
}

BlendRenderTask::~BlendRenderTask()
{
}

void BlendRenderTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	this->commandInterface->Reset(this->commandInterfaceIndex);

	commandList->SetGraphicsRootSignature(this->rootSig);
	
	ID3D12DescriptorHeap* bindlessHeap = this->descriptorHeap_CBV_UAV_SRV->GetID3D12DescriptorHeap();
	commandList->SetDescriptorHeaps(1, &bindlessHeap);

	commandList->SetGraphicsRootDescriptorTable(RS::dtCBV, this->descriptorHeap_CBV_UAV_SRV->GetGPUHeapAt(0));
	commandList->SetGraphicsRootDescriptorTable(RS::dtSRV, this->descriptorHeap_CBV_UAV_SRV->GetGPUHeapAt(0));

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

	// Create a CB_PER_FRAME struct
	CB_PER_FRAME_STRUCT perFrame = { camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z };
	commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_FRAME_CONSTANTS, sizeof(CB_PER_FRAME_STRUCT) / sizeof(UINT), &perFrame, 0);

	commandList->SetGraphicsRootConstantBufferView(RS::CB_PER_SCENE, this->resources[0]->GetGPUVirtualAdress());

	const XMFLOAT4X4* const viewProjMat = this->camera->GetViewProjMatrix();
	XMMATRIX tmpViewProjMat = XMLoadFloat4x4(viewProjMat);

	// Draw from opposite order from the sorted array
	for(int i = this->renderComponents.size() - 1; i >= 0; i--)
	{
		component::MeshComponent* mc = this->renderComponents.at(i).first;
		component::TransformComponent* tc = this->renderComponents.at(i).second;

		// Check if the renderComponent is to be drawn in Blend
		if (mc->GetDrawFlag() & DRAW_FLAG::Blend)
		{
			// Draw for every mesh the MeshComponent has
			for (unsigned int j = 0; j < mc->GetNrOfMeshes(); j++)
			{
				size_t num_Indices = mc->GetMesh(j)->GetNumIndices();
				const SlotInfo* info = mc->GetMesh(j)->GetSlotInfo();

				Transform* transform = tc->GetTransform();
				const XMFLOAT4X4* worldMat = transform->GetWorldMatrix();
				XMFLOAT4X4 WVPTransposed;
				XMFLOAT4X4 wTransposed;

				XMMATRIX tmpWorldMat = XMLoadFloat4x4(worldMat);
				XMMATRIX tmpWVP = tmpWorldMat * tmpViewProjMat;

				// Store and transpose the matrices for shader
				XMStoreFloat4x4(&WVPTransposed, XMMatrixTranspose(tmpWVP));
				XMStoreFloat4x4(&wTransposed, XMMatrixTranspose(tmpWorldMat));

				// Create a CB_PER_OBJECT struct
				CB_PER_OBJECT_STRUCT perObject = { wTransposed, WVPTransposed, *info };

				commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_OBJECT_CONSTANTS, sizeof(CB_PER_OBJECT_STRUCT) / sizeof(UINT), &perObject, 0);

				commandList->IASetIndexBuffer(mc->GetMesh(j)->GetIndexBufferView());
				// Draw each object twice with different PSO 
				for (int k = 0; k < 2; k++)
				{
					commandList->SetPipelineState(this->pipelineStates[k]->GetPSO());
					commandList->DrawIndexedInstanced(num_Indices, 1, 0, 0, 0);
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
