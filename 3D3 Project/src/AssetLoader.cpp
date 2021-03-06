#include "AssetLoader.h"
#include <iostream>

AssetLoader::~AssetLoader()
{
	for (auto pair : this->loadedMeshes)
		delete pair.second;

	for (auto shader : this->loadedShaders)
		delete shader.second;
}

AssetLoader& AssetLoader::Get()
{
	static AssetLoader instance;

	return instance;
}

void AssetLoader::SetDevice(ID3D12Device5* device)
{
	this->device = device;
}

Mesh* AssetLoader::LoadMesh(std::wstring path, bool* loadedBefore)
{
	// Check if the mesh allready exists
	if (this->loadedMeshes.count(path) != 0)
	{
		*loadedBefore = true;
		return this->loadedMeshes[path];
	}

	bool meshLoaded = false;
	bool endOfFile = false;

	// Vectors to hold the indexes of the faces in the objFile.
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	// Vectors to temporary store the attributes of each face.
	std::vector<DirectX::XMFLOAT3> temp_vertices;
	std::vector<DirectX::XMFLOAT2> temp_uvs;
	std::vector<DirectX::XMFLOAT3> temp_normals;

	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	std::string converted = converter.to_bytes(path);

	// Read the filePath and check if we can open it
	FILE* file;
	fopen_s(&file, converted.c_str(), "r");
	if (file == NULL)
	{
		std::cout << "[ERROR] - Cannot open the obj File" << std::endl;
		meshLoaded = false;
	}
	else
	{
		// Read the file and save the data
		while (!endOfFile)
		{
			char firstWordOfLine[128];
			int wordSize = fscanf_s(file, "%s", firstWordOfLine, sizeof(firstWordOfLine));
			if (wordSize == EOF)
			{
				// There is nothing left to be read
				meshLoaded = true;
				endOfFile = true;
			}
			else
			{
				// Find the value of each "v", "vt", "vt" or "f" and store them in the temp vectors
				if (strcmp(firstWordOfLine, "v") == 0)
				{
					DirectX::XMFLOAT3 vertex;
					fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(firstWordOfLine, "vt") == 0)
				{
					DirectX::XMFLOAT2 uv;
					fscanf_s(file, "%f %f\n", &uv.x, &uv.y);

					uv.y = 1 - uv.y;
					temp_uvs.push_back(uv);
				}
				else if (strcmp(firstWordOfLine, "vn") == 0)
				{
					DirectX::XMFLOAT3 normal;
					fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(firstWordOfLine, "f") == 0)
				{
					// find all the indices
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int nrOfIndices = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

					if (nrOfIndices != 9)
					{
						// Our mesh loader is too simple to be able to deal with an obj file like this
						std::cout << "[ERROR] Wrong file format!" << std::endl;
						meshLoaded = false;
						break;
					}

					// Store the indices
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
			}
		}

		unsigned int vertexIndex = 0;
		unsigned int uvIndex = 0;
		unsigned int normalIndex = 0;
		DirectX::XMFLOAT3 vertex;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;

		Mesh::Vertex tempVertex;
		std::vector<Mesh::Vertex> vvertices;


		// Now we store the attributes in the i
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			// Find the index
			vertexIndex = vertexIndices[i];
			uvIndex = uvIndices[i];
			normalIndex = normalIndices[i];

			// Find the value of that index	(-1 because all mesh files starts their indexing at 1 instead of 0)
			vertex = temp_vertices[vertexIndex - 1];
			uv = temp_uvs[uvIndex - 1];
			normal = temp_normals[normalIndex - 1];

			// Store the values in the vectors we recieve in the function
			tempVertex.pos = DirectX::XMFLOAT4(vertex.x, vertex.y, vertex.z, 1.0);
			tempVertex.uv = DirectX::XMFLOAT4(uv.x, uv.y, 0.0, 0.0);
			tempVertex.nor = DirectX::XMFLOAT4(normal.x, normal.y, normal.z, 0.0);

			// Push to Mesh
			vvertices.push_back(tempVertex);
		}

		UINT size = vertexIndices.size() * sizeof(Mesh::Vertex);
		Mesh* newMesh = new Mesh(this->device, vvertices, size, this->slotCounter++);

		this->loadedMeshes[path] = newMesh;

		*loadedBefore = false;
		return newMesh;
	}

	return nullptr;
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

