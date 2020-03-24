#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include "Engine/Mesh.h"
#include "Engine/Shader.h"

class AssetLoader
{
public:
    ~AssetLoader();

    static AssetLoader& Get();

    void SetDevice(ID3D12Device5* device);

    /* Load Functions */
    // Mesh ---------------
    Mesh* LoadMesh(std::wstring path, bool* loadedBefore);

    // Texture ------------
    //Texture& LoadTexture(std::wstring path);


    // Material -----------
    //Material& LoadMaterial(std::wstring path);

    // Shader -------------
    Shader* LoadShader(std::wstring fileName, ShaderType type);

    // Scene --------------


private:
    AssetLoader() {};
    AssetLoader(AssetLoader const&) = delete;
    void operator=(AssetLoader const&) = delete;

    ID3D12Device5* device;

    std::map<std::wstring, Mesh*> loadedMeshes;
    //std::map<std::wstring, Mesh*> loadedTextures;
    //std::map<std::wstring, Mesh*> loadedMaterials;
    std::map<std::wstring, Shader*> loadedShaders;

    unsigned int slotCounter = 0;

    std::wstring filePathShaders = L"src/Engine/HLSL/";
};

#endif