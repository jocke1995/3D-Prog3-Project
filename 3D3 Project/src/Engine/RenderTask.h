#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "DX12Task.h"

#include "DepthBuffer.h"
#include "RenderTarget.h"
#include "Camera.h"
#include "GraphicsState.h"
#include "DescriptorHeap.h"

#include "../ECS/Components/MeshComponent.h"
#include "../ECS/Components/TransformComponent.h"


class RenderTask : public DX12Task
{
public:
	RenderTask(ID3D12Device5* device, 
		RootSignature* rootSignature, 
		LPCWSTR VSName, LPCWSTR PSName, 
		std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> *gpsds,
		LPCTSTR psoName);
	
	virtual ~RenderTask();

	PipelineState* GetPipelineState(unsigned int index);

	void AddResource(std::string id, Resource* resource);
	void AddRenderTarget(RenderTarget* renderTarget);

	void SetRenderComponents(std::vector<std::pair<	component::MeshComponent*,
													component::TransformComponent*>>* renderComponents);

	void SetCamera(Camera* camera);
	void SetDescriptorHeap_CBV_UAV_SRV(DescriptorHeap* dh);
	void SetDepthBuffer(DepthBuffer* depthBuffer);

protected:
	DescriptorHeap* descriptorHeap_CBV_UAV_SRV = nullptr;

	std::map<std::string, Resource*> resources;
	std::vector<RenderTarget*> renderTargets;
	std::vector<PipelineState*> pipelineStates;

	DepthBuffer* depthBuffer = nullptr;
	ID3D12RootSignature* rootSig = nullptr;
	
	Camera* camera = nullptr;

	std::vector<std::pair<component::MeshComponent*, component::TransformComponent*>> renderComponents;

};

#endif