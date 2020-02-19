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

