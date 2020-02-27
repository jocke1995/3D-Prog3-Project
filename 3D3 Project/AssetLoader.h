#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include "Engine/Mesh.h"
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <DirectXMath.h>

// For wstring convertion
#include <locale>
#include <codecvt>




class AssetLoader
{
public:
    ~AssetLoader();

    static AssetLoader& Get();

    /* Load Functions */
    // Mesh ---------------
    Mesh* LoadMesh(std::wstring path);

    // Texture ------------
    //Texture& LoadTexture(std::wstring path);


    // Material -----------
    //Material& LoadMaterial(std::wstring path);

    // Shader -------------
    //Shader& LoadShader(std::wstring path);

    // Scene --------------


private:
    AssetLoader() {};
    AssetLoader(AssetLoader const&) = delete;
    void operator=(AssetLoader const&) = delete;

    std::map<std::wstring, Mesh*> loadedMeshes;
    std::map<std::wstring, Mesh*> loadedTextures;
    std::map<std::wstring, Mesh*> loadedMaterials;
    std::map<std::wstring, Mesh*> loadedShaders;
};




#endif