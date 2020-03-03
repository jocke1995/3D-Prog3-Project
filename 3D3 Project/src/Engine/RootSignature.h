#ifndef ROOTSIGNATURE_H
#define ROOTSIGNATURE_H

#include "stdafx.h"

// TODO: Fyll upp senare när vi vet vad vi ska ha för saker.
enum RS
{
	dtSRV,
	CBV_PER_OBJECT,
	NUM_PARAMS
};

class RootSignature
{
public:
	RootSignature(ID3D12Device5* device);
	~RootSignature();

	ID3D12RootSignature* GetRootSig();
	ID3DBlob* GetBlob();
private:
	// Rootsignature
	ID3D12RootSignature* rootSig;
	ID3DBlob* sBlob;
	void CreateRootSignatureStructure();
};

#endif