#ifndef RENDERER_H
#define RENDERER_H

#include "RootSignature.h"
#include "RenderTask.h"
#include "SwapChain.h"
#include "ThreadPool.h"
#include "Camera.h"
#include "LightConstantBufferPool.h"
#include "ConstantBufferUpload.h"
#include "../ECS/Scene.h"

// lights
#include "../ECS/Components/Lights/DirectionalLightComponent.h"
#include "../ECS/Components/Lights/PointLightComponent.h"
#include "../ECS/Components/Lights/SpotLightComponent.h"

// Graphics
#include "ForwardRenderTask.h"
#include "BlendRenderTask.h"

// Copy
#include "CopyPerFrameTask.h"

// Compute (Later include the specific task, not this)
#include "ComputeTask.h"

extern unsigned int globalDescriptorHeapIndex;

class Renderer
{
public:
	Renderer();
	~Renderer();

	// Call once
	void InitD3D12(const HWND *hwnd, HINSTANCE hInstance);

	std::vector<Mesh*>* LoadModel(std::wstring path);
	Texture* LoadTexture(std::wstring path);

	// Change active scene
	void SetSceneToDraw(Scene* scene);

	// Call each frame
	void Update(double dt);
	void SortObjectsByDistance();
	void Execute();

	ThreadPool* GetThreadPool() const;
	Camera* GetCamera() const;
private:
	// Camera
	Camera* camera = nullptr;
	void SetRenderTasksMainCamera(Camera *camera);

	unsigned int frameCounter = 0;

	// Device
	ID3D12Device5* device5 = nullptr;
	bool CreateDevice();

	// CommandQueues
	std::map<COMMAND_INTERFACE_TYPE, ID3D12CommandQueue*> commandQueues;
	void CreateCommandQueues();

	// Swapchain
	SwapChain* swapChain = nullptr;
	void CreateSwapChain(const HWND *hwnd);

	// Depthbuffer
	DepthBuffer* depthBuffer = nullptr;
	void CreateDepthBuffer();

	// Rootsignature
	RootSignature* rootSignature = nullptr;
	void CreateRootSignature();

	// ThreadPool
	ThreadPool* threadpool = nullptr;

	// RenderTasks
	std::vector<RenderTask*>  renderTasks;
	std::vector<CopyTask*>    copyTasks;
	std::vector<ComputeTask*> computeTasks;
	void InitRenderTasks();

	// Group of components that's needed for rendering:
	std::vector<std::pair<component::MeshComponent*, component::TransformComponent*>> renderComponents;
	void SetRenderTasksRenderComponents();

	LightConstantBufferPool* lightCBPool = nullptr;
	std::vector<std::pair<component::DirectionalLightComponent*, ConstantBufferDefault*>> directionalLights;
	std::vector<std::pair<component::PointLightComponent*, ConstantBufferDefault*>> pointLights;
	std::vector<std::pair<component::SpotLightComponent*, ConstantBufferDefault*>> spotLights;

	// Current scene to be drawn
	Scene* currActiveScene = nullptr;
	ConstantBufferDefault* cbPerScene = nullptr;

	// update per frame
	ConstantBufferDefault* cbPerFrame = nullptr;
	CB_PER_FRAME_STRUCT* cbPerFrameData = nullptr;

	// Commandlists holders
	std::vector<ID3D12CommandList*> copyCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> computeCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> directCommandLists[NUM_SWAP_BUFFERS];
	
	// DescriptorHeap
	DescriptorHeap* descriptorHeap_CBV_UAV_SRV = nullptr;
	DescriptorHeap* descriptorHeap_RTV = nullptr;
	DescriptorHeap* descriptorHeap_DSV = nullptr;
	void InitDescriptorHeaps();

	// Views
	void CreateShaderResourceView(	
		unsigned int descriptorHeapIndex,
		D3D12_SHADER_RESOURCE_VIEW_DESC* desc,
		const Resource* resource);

	bool CreateSRVForTexture(Texture* texture);

	// Fences
	HANDLE eventHandle = nullptr;
	ID3D12Fence1* fenceFrame = nullptr;
	UINT64 fenceFrameValue = 0;
	void CreateFences();

	void WaitForFrame(unsigned int framesToBeAhead = NUM_SWAP_BUFFERS - 1);

	// Temporary.. these functions and variables are used to copy data to GPU on initialization
	void WaitForGpu();
	CommandInterface* tempCommandInterface = nullptr;
	void TempCopyResource(Resource* uploadResource, Resource* defaultResource, void* data);
};

#endif