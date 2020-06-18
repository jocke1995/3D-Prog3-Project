#ifndef GRAPHICSSTATE_H
#define GRAPHICSSTATE_H

#include "PipelineState.h"
class GraphicsState : public PipelineState
{
public:
	GraphicsState(ID3D12Device5* device,
		RootSignature* rootSignature,
		LPCWSTR VSName, LPCWSTR PSName,	// shaderNames
		D3D12_GRAPHICS_PIPELINE_STATE_DESC* gpsd,
		LPCTSTR psoName);	// name of the PSO

	virtual ~GraphicsState();

	const D3D12_GRAPHICS_PIPELINE_STATE_DESC* GetGpsd() const;
	Shader* GetShader(ShaderType type) const;
private:
	// For Graphics
	Shader* VS = nullptr;
	Shader* PS = nullptr;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsd = {};
};

#endif 