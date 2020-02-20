#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"

enum class ShaderType { VS = 0, PS = 1};

class Shader
{
public:
	Shader(LPCTSTR fileName, LPCTSTR filePath, ShaderType type);
	~Shader();


private:
	ID3DBlob* blob;
	LPCTSTR fileName;	// Ex: vertexShader1
	LPCTSTR filePath;	// Ex: ../HLSL/vertexshader1.hlsl"

	bool CompileShader(ShaderType type);

};

#endif