#ifndef ROOTSIGNATURE_H
#define ROOTSIGNATURE_H


enum RS
{
	dtSRV,
	CB_PER_OBJECT_CONSTANTS,
	ColorCBV,
	ColorUAV,
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