#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"

#include "../src/Engine/BaseCamera.h"
class Scene
{
public:
	Scene(BaseCamera* camera);
	~Scene();

	Entity* AddEntity(std::string entityName);
	bool RemoveEntity(std::string entityName);

	Entity* GetEntity(std::string entityName);
	const std::map<std::string, Entity*> *GetEntities() const;

	unsigned int GetNrOfEntites() const;
	BaseCamera* GetMainCamera() const;

	void UpdateScene(double dt);
	
private:

	std::map<std::string, Entity*> entities;
	unsigned int nrOfEntities = 0;

	BaseCamera* mainCamera = nullptr;

	void UpdateEntities(double dt);
	bool EntityExists(std::string entityName) const;
};

#endif
