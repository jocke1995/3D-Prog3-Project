#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include <map>

class Scene
{
public:
	Scene();
	~Scene();

	bool AddEntity(std::string entityName);
	bool RemoveEntity(std::string entityName);

	Entity* GetEntity(std::string entityName);
	std::map<std::string, Entity*> *GetEntities();

	unsigned int GetNrOfEntites();

	void UpdateEntities();
private:
	std::map<std::string, Entity*> entities;
	unsigned int nrOfEntities = 0;

	bool EntityExists(std::string entityName);
};

#endif
