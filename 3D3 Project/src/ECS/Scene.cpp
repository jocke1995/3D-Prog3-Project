#include "Scene.h"

Scene::Scene(BaseCamera* camera)
{
    this->mainCamera = camera;
}

Scene::~Scene()
{
    for (auto pair : this->entities)
    {
        delete pair.second;
    }
    this->entities.clear();
}

// Returns false if the entity couldn't be created
bool Scene::AddEntity(std::string entityName)
{
    if (this->EntityExists(entityName) == true)
    {
        return false;
    }

    this->entities[entityName] = new Entity();
    this->nrOfEntities++;
    return true;
}

bool Scene::RemoveEntity(std::string entityName)
{
    if (this->EntityExists(entityName) == false)
    {
        return false;
    }

    delete this->entities[entityName];
    this->entities.erase(entityName);

    this->nrOfEntities--;
    return true;
}

Entity* Scene::GetEntity(std::string entityName)
{
    if (this->EntityExists(entityName))
    {
        return this->entities.at(entityName);
    }

    Log::PrintSeverity(Log::Severity::CRITICAL, "No Entity with name: \'%s\' was found.\n", entityName.c_str());
    return nullptr;
}

const std::map<std::string, Entity*>* Scene::GetEntities() const
{
	return &this->entities;
}

unsigned int Scene::GetNrOfEntites() const
{
    return this->nrOfEntities;
}

BaseCamera* Scene::GetMainCamera() const
{
	return this->mainCamera;
}

void Scene::UpdateScene(double dt)
{
    this->mainCamera->Update(dt);
    this->UpdateEntities(dt);
}

void Scene::UpdateEntities(double dt)
{
    for (auto pair : this->entities)
    {
        pair.second->Update(dt);
    }
}

bool Scene::EntityExists(std::string entityName) const
{
    for (auto pair : this->entities)
    {
        // An entity with this name already exists
        if (pair.first == entityName)
        {
            return true;
        }
    }
    
    return false;
}
