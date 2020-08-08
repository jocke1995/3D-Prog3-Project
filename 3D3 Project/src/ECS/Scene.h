#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "../Engine/OrthographicCamera.h"
#include "../Engine/PerspectiveCamera.h"
#include <map>

class Scene
{
public:
	Scene(PerspectiveCamera* camera);
	~Scene();

	bool AddEntity(std::string entityName);
	bool RemoveEntity(std::string entityName);

	Entity* GetEntity(std::string entityName);
	const std::map<std::string, Entity*> *GetEntities() const;

	unsigned int GetNrOfEntites() const;
	PerspectiveCamera* GetMainCamera() const;

	void UpdateScene(double dt);
	
private:

	std::map<std::string, Entity*> entities;
	unsigned int nrOfEntities = 0;

	PerspectiveCamera* mainCamera = nullptr;

	void UpdateEntities();
	bool EntityExists(std::string entityName) const;
};

#endif
