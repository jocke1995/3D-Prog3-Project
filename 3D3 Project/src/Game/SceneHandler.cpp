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

void SceneHandler::CreateScene(std::string sceneName, Camera* camera)
{
	this->scenes[sceneName] = new Scene(camera);
}

Scene* SceneHandler::GetScene(std::string sceneName) const
{
    if (this->SceneExists(sceneName))
    {
        return this->scenes.at(sceneName);
    }

    Log::PrintError(Log::ErrorType::GAME, "No Scene with name: \'%s\' was found.\n", sceneName);
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
