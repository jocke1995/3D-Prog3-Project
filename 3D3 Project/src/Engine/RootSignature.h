#ifndef ROOTSIGNATURE_H
#define ROOTSIGNATURE_H

#include "stdafx.h"

// TODO: Fyll upp senare n�r vi vet vad vi ska ha f�r saker.
enum RS
{
	POSITION
};

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