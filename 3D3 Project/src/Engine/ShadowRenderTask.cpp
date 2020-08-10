#include "ShadowRenderTask.h"

ShadowRenderTask::ShadowRenderTask(
	ID3D12Device5* device,
	RootSignature* rootSignature,
	LPCWSTR VSName, LPCWSTR PSName,
	std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*>* gpsds,
	LPCTSTR psoName)
	:RenderTask(device, rootSignature, VSName, PSName, gpsds, psoName)
{

}

ShadowRenderTask::~ShadowRenderTask()
{
}

void ShadowRenderTask::AddShadowCastingLight(std::pair<Light*, ShadowInfo*> light)
{
	this->lights.push_back(light);
}

void ShadowRenderTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);
	this->commandInterface->Reset(this->commandInterfaceIndex);
	
	commandList->SetGraphicsRootSignature(this->rootSig);

	DescriptorHeap* descriptorHeap_CBV_UAV_SRV = this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::CBV_UAV_SRV];
	ID3D12DescriptorHeap* d3d12DescriptorHeap = descriptorHeap_CBV_UAV_SRV->GetID3D12DescriptorHeap();
	commandList->SetDescriptorHeaps(1, &d3d12DescriptorHeap);

	commandList->SetGraphicsRootDescriptorTable(RS::dtSRV, descriptorHeap_CBV_UAV_SRV->GetGPUHeapAt(0));

	DescriptorHeap* depthBufferHeap = this->descriptorHeaps[DESCRIPTOR_HEAP_TYPE::DSV];

	for (auto pair : this->lights)
	{
		commandList->SetPipelineState(this->pipelineStates[0]->GetPSO());
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		const D3D12_VIEWPORT* viewPort = pair.second->GetRenderView()->GetViewPort();
		const D3D12_RECT* rect = pair.second->GetRenderView()->GetScissorRect();
		commandList->RSSetViewports(1, viewPort);
		commandList->RSSetScissorRects(1, rect);

		XMMATRIX* viewProjMat = /*this->camera->GetViewProjection();*/pair.first->GetCamera()->GetViewProjection();

		XMFLOAT4X4 test;
		XMStoreFloat4x4(&test, *viewProjMat);

		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			pair.second->GetResource()->GetID3D12Resource1(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_DEPTH_WRITE));

		// Clear and set depthstencil
		unsigned int dsvIndex = pair.second->GetDSV()->GetDescriptorHeapIndex();
		D3D12_CPU_DESCRIPTOR_HANDLE dsh = depthBufferHeap->GetCPUHeapAt(dsvIndex);
		commandList->ClearDepthStencilView(dsh, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		commandList->OMSetRenderTargets(0, nullptr, true, &dsh);

		// Draw for every Rendercomponent
		for (int i = 0; i < this->renderComponents.size(); i++)
		{
			component::MeshComponent* mc = this->renderComponents.at(i).first;
			component::TransformComponent* tc = this->renderComponents.at(i).second;

			// Check if the object is to be drawn in ShadowPass
			if (mc->GetDrawFlag() & DRAW_FLAG::Shadow)
			{
				// Draw for every mesh the meshComponent has
				for (unsigned int i = 0; i < mc->GetNrOfMeshes(); i++)
				{
					size_t num_Indices = mc->GetMesh(i)->GetNumIndices();
					const SlotInfo* info = mc->GetMesh(i)->GetSlotInfo();

					Transform* transform = tc->GetTransform();
					XMMATRIX* worldMat = transform->GetWorldMatrix();
					XMMATRIX WVP = (*worldMat) * (*viewProjMat);

					// Transpose
					XMMATRIX WVPTransposed = XMMatrixTranspose(WVP);
					XMMATRIX* WTransposed = transform->GetWorldMatrixTransposed();

					// Create a CB_PER_OBJECT struct
					CB_PER_OBJECT_STRUCT perObject = { *WTransposed, WVPTransposed, *info };

					commandList->SetGraphicsRoot32BitConstants(RS::CB_PER_OBJECT_CONSTANTS, sizeof(CB_PER_OBJECT_STRUCT) / sizeof(UINT), &perObject, 0);

					commandList->IASetIndexBuffer(mc->GetMesh(i)->GetIndexBufferView());
					commandList->DrawIndexedInstanced(num_Indices, 1, 0, 0, 0);
				}
			}
		}

		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			pair.second->GetResource()->GetID3D12Resource1(),
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
	commandList->Close();
}