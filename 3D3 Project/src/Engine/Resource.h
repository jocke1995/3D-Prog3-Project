#pragma once

#include "stdafx.h"

class Resource
{
public:
    Resource(ID3D12Device* device, unsigned long long entrySize, std::wstring name = L"RESOURCE_NONAME");
    Resource(std::wstring name = L"RESOURCE_NONAME"); // Just set default variables
    virtual ~Resource();

    void SetData(const void* data);

    size_t GetSize();

    ID3D12Resource1* GetID3D12Resource1();

protected:
    std::wstring name;
    unsigned long long entrySize = 0;
    ID3D12Resource1* resource = nullptr;

};