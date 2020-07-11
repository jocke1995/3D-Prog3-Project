#ifndef SCENEHANDLER_H
#define SCENEHANDLER_H

#include "Scene.h"
#include "../Engine/Renderer.h"

class SceneHandler 
{
public:
	SceneHandler();
	~SceneHandler();

	void CreateScene(std::string sceneName, Camera* camera);

	Scene* GetScene(std::string sceneName) const;

private:
	std::map<std::string, Scene*> scenes;

	bool SceneExists(std::string sceneName) const;
};

#endif
