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

	// For every texture
	for (auto pair : this->loadedTextures)
	{
		delete pair.second;
	}

	// For every shader
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

	const aiScene* assimpScene = importer.ReadFile(filePath, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (assimpScene == nullptr)
	{

		Log::PrintError(Log::ErrorType::ENGINE, "Failed to load model with path: \'%s\'\n", filePath.c_str());
		return nullptr;
	}
	
	std::vector<Mesh*> *meshes = new std::vector<Mesh*>;
	meshes->reserve(assimpScene->mNumMeshes);
	this->loadedModels[path] = meshes;

	this->ProcessNode(assimpScene->mRootNode, assimpScene, meshes);

	*loadedBefore = false;
	return this->loadedModels[path];
}

Texture* AssetLoader::LoadTexture(std::wstring path, bool* loadedBefore)
{
	*loadedBefore = false;

	// Check if the texture allready exists
	if (this->loadedTextures.count(path) != 0)
	{
		*loadedBefore = true;
		return this->loadedTextures[path];
	}

	Texture* texture = new Texture(path, this->device);

	this->loadedTextures[path] = texture;
	return this->loadedTextures[path];
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
		
		//aiMaterial* mat = assimpScene->mMaterials[mesh->mMaterialIndex];
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
	std::vector<unsigned int> indices;

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
			Log::PrintError(Log::ErrorType::ENGINE, "Mesh has no positions");
		}

		// Get Normals
		if (assimpMesh->HasNormals())
		{
			vTemp.normal.x = assimpMesh->mNormals[i].x;
			vTemp.normal.y = assimpMesh->mNormals[i].y;
			vTemp.normal.z = assimpMesh->mNormals[i].z;
			vTemp.normal.w = 0.0;
		}
		else
		{
			Log::PrintError(Log::ErrorType::ENGINE, "Mesh has no normals");
		}

		if (assimpMesh->HasTangentsAndBitangents())
		{
			// Todo: Add bitangent if needed
			vTemp.tangent.x = assimpMesh->mTangents[i].x;
			vTemp.tangent.y = assimpMesh->mTangents[i].y;
			vTemp.tangent.z = assimpMesh->mTangents[i].z;
			vTemp.tangent.w = 0.0;
		}
		else
		{
			Log::PrintError(Log::ErrorType::ENGINE, "Mesh has no tangents");
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
			Log::PrintError(Log::ErrorType::ENGINE, "Mesh has no textureCoords");
		}

		vertices.push_back(vTemp);
	}

	// Get indices
	for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++)
	{
		aiFace face = assimpMesh->mFaces[i];
	
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	return new Mesh(this->device, vertices, indices, descriptorHeapIndex++);
}
