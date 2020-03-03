#ifndef SHADER_H
#define SHADER_H

#include "stdafx.h"

enum class ShaderType { VS = 0, PS = 1 };

class Shader
{
public:
	Shader(LPCTSTR path, ShaderType type);
	~Shader();

	ID3DBlob* GetBlob();

private:
	ID3DBlob* blob;
	ShaderType type;
	LPCTSTR path;	// Ex: vertexShader1

	void CompileShader();
};

#endif