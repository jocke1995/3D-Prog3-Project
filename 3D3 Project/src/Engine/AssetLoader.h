#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

static unsigned int descriptorHeapIndex_SRV = 0;

class AssetLoader
{
public:
    ~AssetLoader();

    static AssetLoader* Get(ID3D12Device5* device = nullptr);

    /* Load Functions */
    // Mesh ---------------
    std::vector<Mesh*>* LoadModel(const std::wstring path, bool* loadedBefore);

    // Texture ------------
    Texture* LoadTexture(std::wstring path);


    // Material -----------
    //Material& LoadMaterial(std::wstring path);

    // Shader -------------
    Shader* LoadShader(std::wstring fileName, ShaderType type);

private:
    AssetLoader(ID3D12Device5* device = nullptr);
    AssetLoader(AssetLoader const&) = delete;
    void operator=(AssetLoader const&) = delete;

    ID3D12Device5* device;

    std::map<std::wstring, std::vector<Mesh*>*> loadedModels;
    void ProcessNode(aiNode* node, const aiScene* assimpScene, std::vector<Mesh*> *meshes, const std::string* filePath);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* assimpScene, const std::string* filePath);
    Texture* ProcessTexture(aiMaterial* mat, TEXTURE_TYPE texture_type, const std::string* filePathWithoutTexture);

    std::map<std::wstring, Texture*> loadedTextures;
    //std::map<std::wstring, Mesh*> loadedMaterials;
    std::map<std::wstring, Shader*> loadedShaders;

    std::wstring filePathShaders = L"src/Engine/HLSL/";
    std::wstring filePathDefaultTextures = L"Resources/Textures/Default/";
};

#endif