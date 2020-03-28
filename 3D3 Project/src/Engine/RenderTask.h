#ifndef RENDERTASK_H
#define RENDERTASK_H

#include "PipelineState.h"
#include "Object.h"
#include "RenderTarget.h"
#include "RootSignature.h"
#include "Camera.h"
#include "DepthBuffer.h"
#include "D3D12Timer.h"
#include "DX12Task.h"
#include "CommandInterface.h"

class DescriptorHeap;

class RenderTask : public DX12Task
{
public:
	RenderTask(ID3D12Device5* device, 
		RootSignature* rootSignature, 
		LPCWSTR VSName, LPCWSTR PSName, 
		std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC*> *gpsds, 
		COMMAND_INTERFACE_TYPE interfaceType);
	
	virtual ~RenderTask();

	PipelineState* GetPipelineState(unsigned int index);

	void AddRenderTarget(RenderTarget* renderTarget);
	void AddObject(Object* object);

	void SetCamera(Camera* camera);
	void SetDescriptorHeap(DescriptorHeap* dh);
	void SetDepthBuffer(DepthBuffer* depthBuffer);

protected:
	DescriptorHeap* descriptorHeap = nullptr;

	std::vector<RenderTarget*> renderTargets;
	DepthBuffer* depthBuffer = nullptr;
	ID3D12RootSignature* rootSig = nullptr;
	
	std::vector<PipelineState*> pipelineStates;
	std::vector<Object*> objects;
	Camera* camera = nullptr;
};

#endif