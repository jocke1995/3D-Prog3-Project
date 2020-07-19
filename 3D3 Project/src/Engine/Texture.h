#ifndef TEXTURE_H
#define TEXTURE_H

#include <wincodec.h>
#include "Resource.h"
class Texture
{
public:
	Texture(std::wstring filePath, ID3D12Device1* device);
	~Texture();

private:
	std::wstring filePath = L"";
	unsigned int bytesPerRow = 0;
	unsigned int imageSize = 0;
	BYTE* imageData = nullptr;

	D3D12_RESOURCE_DESC resourceDescription = {};
	Resource* resourceDefaultHeap = nullptr;
	bool Create(std::wstring filePath, ID3D12Device1* device);
	void CreateDefaultHeap(ID3D12Device1* device);	// This will happen once. No data is copied here.
};

#endif
