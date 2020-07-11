#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "DX12Task.h"

#include "DepthBuffer.h"
#include "RenderTarget.h"
#include "Camera.h"
#include "../Game/Entity.h"
#include "GraphicsState.h"
#include "DescriptorHeap.h"



class RenderTask : public DX12Task
{
public:
	RenderTask(ID3D12Device5* device, 
		RootSignature* rootSignature, 
		LPCWSTR VSName, LPCWSTR PSName, 
		std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> *gpsds,
		LPCTSTR psoName,
		COMMAND_INTERFACE_TYPE interfaceType);
	
	virtual ~RenderTask();

	PipelineState* GetPipelineState(unsigned int index);

	void AddRenderTarget(RenderTarget* renderTarget);

	void SetRenderComponents(std::vector<component::RenderComponent*> * renderComponents);
	void SetdirectionalLightComponents(std::vector<component::DirectionalLightComponent*>* dirLightComponents);

	void SetCamera(Camera* camera);
	void SetDescriptorHeap_CBV_UAV_SRV(DescriptorHeap* dh);
	void SetDepthBuffer(DepthBuffer* depthBuffer);

protected:
	DescriptorHeap* descriptorHeap_CBV_UAV_SRV = nullptr;

	std::vector<RenderTarget*> renderTargets;
	DepthBuffer* depthBuffer = nullptr;
	ID3D12RootSignature* rootSig = nullptr;
	
	std::vector<PipelineState*> pipelineStates;
	Camera* camera = nullptr;

	std::vector<component::RenderComponent*> renderComponents;
	std::vector<component::DirectionalLightComponent*> dirLightComponents;

};

#endif