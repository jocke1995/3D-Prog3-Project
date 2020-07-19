#include "SceneHandler.h"

SceneHandler::SceneHandler()
{
}

SceneHandler::~SceneHandler()
{
    for (auto pair : this->scenes)
    {
        delete pair.second;
    }
    this->scenes.clear();
}

Scene* SceneHandler::CreateScene(std::string sceneName, Camera* camera)
{
    if (this->SceneExists(sceneName))
    {
        Log::PrintError(Log::ErrorType::ENGINE, "A scene with the name: \'%s\' allready exists.\n", sceneName.c_str());
        return nullptr;
    }

    // Create Scene and return it
    this->scenes[sceneName] = new Scene(camera);
    return this->scenes[sceneName];
}

Scene* SceneHandler::GetScene(std::string sceneName) const
{
    if (this->SceneExists(sceneName))
    {
        return this->scenes.at(sceneName);
    }

    Log::PrintError(Log::ErrorType::ENGINE, "No Scene with name: \'%s\' was found.\n", sceneName.c_str());
    return nullptr;
}

bool SceneHandler::SceneExists(std::string sceneName) const
{
    for (auto pair : this->scenes)
    {
        // A Scene with this name already exists
        if (pair.first == sceneName)
        {
            return true;
        }
    }

    return false;
}
