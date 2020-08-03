#pragma once

enum RESOURCE_TYPE
{
    UPLOAD,
    DEFAULT,
    NUM_RESOURCE_TYPES
};

class Resource
{
public:
    Resource(ID3D12Device* device, 
        unsigned long long entrySize, 
        RESOURCE_TYPE type,
        std::wstring name = L"RESOURCE_NONAME",
        D3D12_RESOURCE_DESC* resourceDescInput = nullptr); // Optional, if more precise settings are needed
    virtual ~Resource();

    size_t GetSize() const;
    ID3D12Resource1* GetID3D12Resource1() const;
    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAdress() const;

    void SetData(const void* data, unsigned int subResourceIndex = 0);
    void SetData(const void* data, unsigned int subResourceIndex = 0) const;
protected:
    std::wstring name;
    unsigned long long entrySize = 0;
    ID3D12Resource1* resource = nullptr;

};