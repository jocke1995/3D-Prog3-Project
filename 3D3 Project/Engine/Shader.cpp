#include "Shader.h"

Shader::Shader(LPCTSTR fileName, ShaderType type)
{
	this->fileName = fileName;

	std::wstring tempFilePath = L"Engine/HLSL/";	// TODO: Ska vara i assetloader senare.
	std::wstring tempFileName = fileName;
	std::wstring filePath = tempFilePath + tempFileName;


	if (!this->CompileShader(type, filePath.c_str()))
	{
		// TODO: Errorbox or no? Göra en klass för debugsträngar?
		OutputDebugStringA("Error: Failed to create Shader!\n");
	}
}

Shader::~Shader()
{
	SAFE_RELEASE(&this->blob);
}

ID3DBlob* Shader::GetBlob()
{
	return this->blob;
}

bool Shader::CompileShader(ShaderType type, LPCTSTR filePath)
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
		filePath, // filePath + filename
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

	if (this->blob == nullptr)
	{
		OutputDebugStringA("Error: ShaderBlob is nullptr\n");
	}

	if (FAILED(hr) && errorMessages)
	{
		const char* errorMsg = (const char*)errorMessages->GetBufferPointer();
		// Printa i output
		OutputDebugStringA(errorMsg);
		return false;
	}

	// TODO: KANSKE INTE SKA VARA HÄR
	// SAFE_RELEASE(&errorMessages);
	return true;
}
