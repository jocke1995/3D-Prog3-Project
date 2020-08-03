#ifndef TEXTURE_H
#define TEXTURE_H

#include <wincodec.h>
#include "Resource.h"
#include "CommandInterface.h"

enum TEXTURE_TYPE
{
	AMBIENT,
	DIFFUSE,
	SPECULAR,
	NORMAL,
	EMISSIVE,
	NUM_TEXTURE_TYPES
};

class Texture
{
public:
	Texture();
	~Texture();
	
	bool Init(std::wstring filePath, ID3D12Device5* device, UINT descriptorHeapIndex_SRV);
	void UploadTextureData(ID3D12Device5* device, CommandInterface* commandInterface, ID3D12CommandQueue* cmdQueue);

	// Call only once! Todo: bad code, fix fix fix
	void Bind();

	const UINT GetDescriptorHeapIndex() const;
	Resource* GetResource() const;
	const DXGI_FORMAT* GetGDXIFormat() const;

	bool IsBoundToSRV();

private:
	std::wstring filePath = L"";
	unsigned int bytesPerRow = 0;
	unsigned int imageSize = 0;
	BYTE* imageData = nullptr;
	UINT descriptorHeapIndex_SRV = -1;

	bool hasSRV = false;

	D3D12_RESOURCE_DESC resourceDescription = {};
	Resource* resourceDefaultHeap = nullptr;
	Resource* resourceUploadHeap = nullptr;
	bool CreateTexture(std::wstring filePath, ID3D12Device5* device, UINT descriptorHeapIndex_SRV);
};

#endif
