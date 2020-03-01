#ifndef ROOTSIGNATURE_H
#define ROOTSIGNATURE_H

#include "stdafx.h"

class RootSignature
{
public:
	RootSignature();
	~RootSignature();

	ID3D12RootSignature** GetRootSig();
	ID3DBlob* GetBlob();
private:
	// Rootsignature
	ID3D12RootSignature* rootSig;
	ID3DBlob* sBlob;
	bool CreateRootSignatureStructure();
};

#endif