#ifndef RENDERER_H
#define RENDERER_H

#include "RootSignature.h"
#include "SwapChain.h"
#include "DepthStencilView.h"
#include "ThreadPool.h"
#include "LightViewsPool.h"

#include "MousePicker.h"
#include "BoundingBoxPool.h"

// ECS
#include "../ECS/Scene.h"
#include "../ECS/Components/CameraComponent.h"
#include "../ECS/Components/BoundingBoxComponent.h"

// Graphics
#include "WireframeRenderTask.h"
#include "ForwardRenderTask.h"
#include "BlendRenderTask.h"
#include "ShadowRenderTask.h"

// Copy
#include "CopyPerFrameTask.h"

// Compute (Later include the specific tasks instead of this)
#include "ComputeTask.h"

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
private:
	// Camera
	BaseCamera* ScenePrimaryCamera = nullptr;
	void SetRenderTasksPrimaryCamera();

	unsigned int frameCounter = 0;

	// Device
	ID3D12Device5* device5 = nullptr;
	bool CreateDevice();

	// CommandQueues
	std::map<COMMAND_INTERFACE_TYPE, ID3D12CommandQueue*> commandQueues;
	void CreateCommandQueues();

	// Swapchain (inheriting from 'RenderTarget')
	SwapChain* swapChain = nullptr;
	void CreateSwapChain(const HWND *hwnd);

	// Depthbuffer
	DepthStencilView* mainDSV = nullptr;
	void CreateMainDSV(const HWND* hwnd);

	// Rootsignature
	RootSignature* rootSignature = nullptr;
	void CreateRootSignature();

	// ThreadPool
	ThreadPool* threadpool = nullptr;

	// Picking
	MousePicker* mousePicker = nullptr;

	// Tasks
	std::vector<ComputeTask*> computeTasks;
	std::vector<CopyTask*>    copyTasks;
	std::vector<RenderTask*>  renderTasks;
	WireframeRenderTask* wireFrameTask = nullptr;
	void InitRenderTasks();

	// Group of components that's needed for rendering:
	std::vector<std::pair<component::MeshComponent*, component::TransformComponent*>> renderComponents;
	void SetRenderTasksRenderComponents();

	// BoundingBoxes to be picked
	std::vector<component::BoundingBoxComponent*> boundingBoxesToBePicked;

	LightViewsPool* lightViewsPool = nullptr;
	std::map<LIGHT_TYPE, std::vector<std::tuple<Light*, ConstantBufferView*, ShadowInfo*>>> lights;

	// Current scene to be drawn
	Scene* currActiveScene = nullptr;
	ConstantBufferView* cbPerScene = nullptr;

	// update per frame
	CB_PER_FRAME_STRUCT* cbPerFrameData = nullptr;
	ConstantBufferView* cbPerFrame = nullptr;

	// Commandlists holders
	std::vector<ID3D12CommandList*> directCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> computeCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> copyCommandLists[NUM_SWAP_BUFFERS];
	
	// DescriptorHeaps
	std::map<DESCRIPTOR_HEAP_TYPE, DescriptorHeap*> descriptorHeaps = {};
	void CreateDescriptorHeaps();

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