#include "PipelineState.h"

PipelineState::PipelineState()
{
	
}

PipelineState::~PipelineState()
{
	if (this->VS != nullptr)
		delete this->VS;
	if (this->PS != nullptr)
		delete this->PS;

	SAFE_RELEASE(&this->PSO);
}

void PipelineState::CreateShader(LPCTSTR fileName, ShaderType type)
{
	if (type == ShaderType::VS)
		this->VS = new Shader(fileName, type);
	else if (type == ShaderType::PS)
		this->PS = new Shader(fileName, type);
}

ID3D12PipelineState** PipelineState::GetPSO()
{
	return &this->PSO;
}

Shader* PipelineState::GetShader(ShaderType type)
{
	if (type == ShaderType::VS)
		return this->VS;
	else if (type == ShaderType::PS)
		return this->PS;

	return nullptr;
}

