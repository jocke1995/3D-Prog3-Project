#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "DX12Task.h"

#include "DepthBuffer.h"
#include "RenderTarget.h"
#include "Camera.h"
#include "GraphicsState.h"
#include "SwapChain.h"

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
	void SetDescriptorHeap(DESCRIPTOR_HEAP_TYPE type, DescriptorHeap* dh);
	void SetSwapChain(SwapChain* swapChain);

protected:
	

	std::map<std::string, Resource*> resources;
	std::vector<RenderTarget*> renderTargets;
	std::vector<std::pair<component::MeshComponent*, component::TransformComponent*>> renderComponents;
	
	Camera* camera = nullptr;
	std::map<DESCRIPTOR_HEAP_TYPE, DescriptorHeap*> descriptorHeaps;
	SwapChain* swapChain = nullptr;

	ID3D12RootSignature* rootSig = nullptr;
	std::vector<PipelineState*> pipelineStates;
};

#endif