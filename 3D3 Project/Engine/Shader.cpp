#include "Shader.h"

Shader::Shader(LPCTSTR fileName, ShaderType type)
{
	this->fileName = fileName;
	this->type = type;
}

Shader::~Shader()
{
	SAFE_RELEASE(&this->blob);
}

ID3DBlob** Shader::GetBlob()
{
	return &this->blob;
}

