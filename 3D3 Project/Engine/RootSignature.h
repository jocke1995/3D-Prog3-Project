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
	RootSignature(ID3D12Device5 * device5);
	~RootSignature();

	ID3D12RootSignature* GetRootSig();
private:
	// Rootsignature
	ID3D12RootSignature* rootSig;
	bool CreateRootSignature(ID3D12Device5* device5);
};

#endif