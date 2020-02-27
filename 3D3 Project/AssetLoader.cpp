#include "AssetLoader.h"
#include <iostream>

AssetLoader::~AssetLoader()
{

}

AssetLoader& AssetLoader::Get()
{
	static AssetLoader instance;

	return instance;
}

Mesh* AssetLoader::LoadMesh(std::wstring path)
{
    std::cout << " hejkasanwdn" << std::endl;

	Mesh* a = new Mesh();
    return a;
}

Shader* AssetLoader::LoadShader(std::wstring fileName, ShaderType type)
{
	// Check if the shader allready exists
	if (loadedShaders.count(fileName) != 0)
	{
		return loadedShaders[fileName];
	}
	// else, create a new shader and compile it

	Shader* tempShader = new Shader(fileName.c_str(), type);

	std::wstring entireFilePath = filePathShaders + fileName;

	this->CompileShader(type, entireFilePath.c_str(), tempShader->GetBlob());

	loadedShaders[fileName] = tempShader;
	return loadedShaders[fileName];
}

void AssetLoader::CompileShader(ShaderType type, LPCWSTR entireFilePath, ID3DBlob ** blob)
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
		entireFilePath, // filePath + filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		entryPoint.c_str(),		// entry point
		shaderModelTarget.c_str(),		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		blob,	// double pointer to ID3DBlob		
		&errorMessages			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	if (blob == nullptr)
	{
		OutputDebugStringA("Error: Blob is nullptr when loading shader");
	}

	if (FAILED(hr) && errorMessages)
	{
		const char* errorMsg = (const char*)errorMessages->GetBufferPointer();
		// Printa i output
		OutputDebugStringA(errorMsg);
	}
}
