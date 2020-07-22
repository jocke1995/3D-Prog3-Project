#ifndef RENDERER_H
#define RENDERER_H

#include "RootSignature.h"
#include "RenderTask.h"
#include "SwapChain.h"
#include "ThreadPool.h"
#include "Camera.h"
#include "../ECS/Scene.h"

// Graphics
#include "ForwardRenderTask.h"
#include "BlendRenderTask.h"

// Copy (Later include the specific task, not this)
#include "CopyTask.h"

// Compute (Later include the specific task, not this)
#include "ComputeTask.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void InitD3D12(const HWND *hwnd, HINSTANCE hInstance);

	std::vector<Mesh*>* LoadModel(std::wstring path);
	Texture* LoadTexture(std::wstring path);
	void CreateConstantBufferView(unsigned int descriptorHeapIndex, unsigned int size, Resource* resource);

	void SetSceneToDraw(Scene* scene);
	//bool AddEntityToDraw(Entity* entity);
	//bool RemoveEntityToDraw(Entity* entity);

	void UpdateScene(double dt);
	void SortEntitiesByDistance();
	void Execute();

	ThreadPool* GetThreadPool() const;
	Camera* GetCamera() const;
	ID3D12Device5* GetDevice() const;
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
	RenderTarget* swapChain = nullptr;
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
	std::vector<component::DirectionalLightComponent*> dirLightComponents;
	void SetRenderTasksDirLightComponents();

	// Current scene to be drawn
	Scene* currActiveScene = nullptr;

	// Commandlists holders
	std::vector<ID3D12CommandList*> copyCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> computeCommandLists[NUM_SWAP_BUFFERS];
	std::vector<ID3D12CommandList*> directCommandLists[NUM_SWAP_BUFFERS];
	
	// DescriptorHeap
	DescriptorHeap* descriptorHeap_CBV_UAV_SRV = nullptr;
	void InitDescriptorHeap();

	// Views
	void CreateShaderResourceView(	unsigned int descriptorHeapIndex,
									D3D12_SHADER_RESOURCE_VIEW_DESC* desc,
									Resource* resource);

	bool CreateSRVForTexture(Texture* texture);

	// Fences
	HANDLE eventHandle = nullptr;
	ID3D12Fence1* fenceFrame = nullptr;
	UINT64 fenceFrameValue = 0;
	void CreateFences();

	void WaitForFrame();

	// Temp
	void WaitForGpu();
	CommandInterface* tempCommandInterface = nullptr;
};

#endif