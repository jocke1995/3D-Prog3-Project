#include "AssetLoader.h"
#include <iostream>

AssetLoader::~AssetLoader()
{
	// For every model
	for (auto pair : this->loadedModels)
	{
		// For every mesh the model has
		for (unsigned int i = 0; i < pair.second->size(); i++)
		{
			delete pair.second->at(i);
		}
		delete pair.second;
	}

	for (auto shader : this->loadedShaders)
		delete shader.second;
}

AssetLoader* AssetLoader::Get()
{
	static AssetLoader instance;

	return &instance;
}

void AssetLoader::SetDevice(ID3D12Device5* device)
{
	this->device = device;
}

std::vector<Mesh*>* AssetLoader::LoadModel(const std::wstring path, bool* loadedBefore)
{
	// Check if the model allready exists
	if (this->loadedModels.count(path) != 0)
	{
		*loadedBefore = true;
		return this->loadedModels[path];
	}

	// Else load the model
	const std::string filePath(path.begin(), path.end());
	Assimp::Importer importer;

	const aiScene* assimpScene = importer.ReadFile(filePath, aiProcess_Triangulate);

	if (assimpScene == nullptr)
	{
		// Log failed to load model
		return nullptr;
	}
	
	std::vector<Mesh*> *meshes = new std::vector<Mesh*>;
	this->loadedModels[path] = meshes;

	this->ProcessNode(assimpScene->mRootNode, assimpScene, meshes);

	*loadedBefore = false;
	return this->loadedModels[path];
}

Shader* AssetLoader::LoadShader(std::wstring fileName, ShaderType type)
{
	// Check if the shader allready exists
	if (loadedShaders.count(fileName) != 0)
	{
		return loadedShaders[fileName];
	}
	// else, create a new shader and compile it

	std::wstring entireFilePath = filePathShaders + fileName;
	Shader* tempShader = new Shader(entireFilePath.c_str(), type);

	loadedShaders[fileName] = tempShader;
	return loadedShaders[fileName];
}

void AssetLoader::ProcessNode(aiNode* node, const aiScene* assimpScene, std::vector<Mesh*>* meshes)
{
	// Go through all the meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = assimpScene->mMeshes[node->mMeshes[i]];
		meshes->push_back(this->ProcessMesh(mesh, assimpScene));
	}
	
	// If the node has more node children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], assimpScene, meshes);
	}
}

Mesh* AssetLoader::ProcessMesh(aiMesh* assimpMesh, const aiScene* assimpScene)
{
	// Fill this data
	std::vector<Mesh::Vertex> vertices;

	// Get data from assimpMesh and store it
	for (unsigned int i = 0; i < assimpMesh->mNumVertices; i++)
	{
		Mesh::Vertex vTemp = {};

		// Get positions
		if (assimpMesh->HasPositions())
		{
			vTemp.pos.x = assimpMesh->mVertices[i].x;
			vTemp.pos.y = assimpMesh->mVertices[i].y;
			vTemp.pos.z = assimpMesh->mVertices[i].z;
			vTemp.pos.w = 1.0;
		}
		else
		{
			// Log no positions
		}

		// Get Normals
		if (assimpMesh->HasNormals())
		{
			vTemp.nor.x = assimpMesh->mNormals[i].x;
			vTemp.nor.y = assimpMesh->mNormals[i].y;
			vTemp.nor.z = assimpMesh->mNormals[i].z;
			vTemp.nor.w = 0.0;
		}
		else
		{
			// Log no Normals
			int a = 5;
		}
		
		
		// Get texture coordinates if there are any
		if (assimpMesh->HasTextureCoords(0))
		{
			vTemp.uv.x = (float)assimpMesh->mTextureCoords[0][i].x;
			vTemp.uv.y = (float)assimpMesh->mTextureCoords[0][i].y;
			vTemp.uv.z = 0.0f;
			vTemp.uv.w = 0.0f;
		}
		else
		{
			// Log no uv
		}

		vertices.push_back(vTemp);
	}

	// Get indices
	// for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++)
	// {
	// 	aiFace face = assimpMesh->mFaces[i];
	// 
	// 	for (unsigned int j = 0; j < face.mNumIndices; j++)
	// 	{
	// 		blabla.push_back(face.mIndices[j]);
	// 	}
	// }

	return new Mesh(this->device, vertices, this->slotCounter++);
}
