#include "CopyLightsTask.h"

CopyLightsTask::CopyLightsTask(
	ID3D12Device5* device, 
	std::vector<std::pair<component::DirectionalLightComponent*,ConstantBufferDefault*>>* directionalLights,
	std::vector<std::pair<component::PointLightComponent*, ConstantBufferDefault*>>* pointLights,
	std::vector<std::pair<component::SpotLightComponent*, ConstantBufferDefault*>>* spotLights)
	:CopyTask(device)
{
	this->directionalLights = directionalLights;
	this->pointLights = pointLights;
	this->spotLights = spotLights;
}

CopyLightsTask::~CopyLightsTask()
{

}

void CopyLightsTask::Execute()
{
	ID3D12CommandAllocator* commandAllocator = this->commandInterface->GetCommandAllocator(this->commandInterfaceIndex);
	ID3D12GraphicsCommandList5* commandList = this->commandInterface->GetCommandList(this->commandInterfaceIndex);

	this->commandInterface->Reset(this->commandInterfaceIndex);

	// Copy DirectionalLights
	for (auto& pair : *this->directionalLights)
	{
		this->CopyResource(
			commandList,
			pair.second->GetUploadResource(),
			pair.second->GetDefaultResource(),
			pair.first->GetDirectionalLight());
	}

	// Copy PointLights
	for (auto& pair : *this->pointLights)
	{
		this->CopyResource(
			commandList,
			pair.second->GetUploadResource(),
			pair.second->GetDefaultResource(),
			pair.first->GetPointLight());
	}

	// Copy SpotLights
	for (auto& pair : *this->spotLights)
	{
		this->CopyResource(
			commandList,
			pair.second->GetUploadResource(),
			pair.second->GetDefaultResource(),
			pair.first->GetSpotLight());
	}

	commandList->Close();
}

void CopyLightsTask::CopyResource(
	ID3D12GraphicsCommandList5* commandList,
	Resource* uploadResource, Resource* defaultResource,
	void* data)
{
	// Set the data into the upload heap
	uploadResource->SetData(data);

	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		defaultResource->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_COPY_DEST));

	// To Defaultheap from Uploadheap
	commandList->CopyResource(
		defaultResource->GetID3D12Resource1(),	// Receiever
		uploadResource->GetID3D12Resource1());	// Sender

	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
		defaultResource->GetID3D12Resource1(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_COMMON));
}
