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
}

void PipelineState::CreateShader(LPCTSTR fileName, LPCTSTR filePath, ShaderType type)
{
	if (type == ShaderType::VS)
		this->VS = new Shader(fileName, filePath, type);
	else if (type == ShaderType::PS)
		this->VS = new Shader(fileName, filePath, type);

}

// TODO: Hur ska man göra utan att hårdkoda detta? inparametrar..?
bool PipelineState::CreatePSO()
{
	return false;
}
