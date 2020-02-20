#include "Shader.h"

Shader::Shader(LPCTSTR fileName, LPCTSTR filePath, ShaderType type)
{
	this->fileName = fileName;
	this->filePath = filePath;

	if (!this->CompileShader(type))
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Error: Failed to create Shader!");
	}
}

Shader::~Shader()
{
}

bool Shader::CompileShader(ShaderType type)
{
	std::string entryPoint;
	std::string shaderModelTarget;

	if (type == ShaderType::VS)
	{
		entryPoint = "VS_main";
		shaderModelTarget = "vs_5_0";
	}
	else if (type == ShaderType::PS)
	{
		entryPoint = "PS_main";
		shaderModelTarget = "ps_5_0";
	}

	ID3DBlob* errorMessages = nullptr;

	HRESULT hr = D3DCompileFromFile(
		this->fileName, // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		entryPoint.c_str(),		// entry point
		shaderModelTarget.c_str(),		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&this->blob,	// double pointer to ID3DBlob		
		&errorMessages			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	if (FAILED(hr) && errorMessages)
	{
		const char* errorMsg = (const char*)errorMessages->GetBufferPointer();
		// Printa i output
		OutputDebugStringA(errorMsg);
		return false;
	}
	return true;
}
