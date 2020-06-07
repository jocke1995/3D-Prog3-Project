#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "../Engine/Camera.h"
#include <map>

class Scene
{
public:
	Scene(Camera* mainCamera);
	~Scene();

	bool AddEntity(std::string entityName);
	bool RemoveEntity(std::string entityName);

	Entity* GetEntity(std::string entityName);
	std::map<std::string, Entity*> *GetEntities();

	unsigned int GetNrOfEntites();

	void UpdateScene(double dt);
	
private:
	std::map<std::string, Entity*> entities;
	unsigned int nrOfEntities = 0;

	Camera* mainCamera = nullptr;

	void UpdateEntities();
	bool EntityExists(std::string entityName);
};

#endif
