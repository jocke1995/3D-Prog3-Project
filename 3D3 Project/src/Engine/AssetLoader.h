#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include "Mesh.h"
#include "Shader.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class AssetLoader
{
public:
    ~AssetLoader();

    static AssetLoader& Get();

    void SetDevice(ID3D12Device5* device);

    /* Load Functions */
    // Mesh ---------------
    std::vector<Mesh*> LoadModel(const std::wstring path, bool* loadedBefore);

    // Texture ------------
    //Texture& LoadTexture(std::wstring path);


    // Material -----------
    //Material& LoadMaterial(std::wstring path);

    // Shader -------------
    Shader* LoadShader(std::wstring fileName, ShaderType type);

private:
    AssetLoader() {};
    AssetLoader(AssetLoader const&) = delete;
    void operator=(AssetLoader const&) = delete;

    ID3D12Device5* device;

    std::map<std::wstring, std::vector<Mesh*>*> loadedModels;
    void ProcessNode(aiNode* node, const aiScene* assimpScene, std::vector<Mesh*> *meshes);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* assimpScene);

    //std::map<std::wstring, Mesh*> loadedTextures;
    //std::map<std::wstring, Mesh*> loadedMaterials;
    std::map<std::wstring, Shader*> loadedShaders;

    unsigned int slotCounter = 0;

    std::wstring filePathShaders = L"src/Engine/HLSL/";
};

#endif