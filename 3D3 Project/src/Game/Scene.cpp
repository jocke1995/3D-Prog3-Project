#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
    for (auto const& [entityName, entity] : this->entities)
    {
        delete entity;
    }
}

// Returns -1 if the entity couldn't be created
// Returns nrOfEntites in scene otherwise
bool Scene::AddEntity(std::string entityName)
{
    if (this->EntityExists(entityName) == true)
    {
        return false;
    }

    this->entities[entityName] = new Entity();
    return true;
}

bool Scene::RemoveEntity(std::string entityName)
{
    if (this->EntityExists(entityName) == true)
    {
        return false;
    }

    delete this->entities[entityName];
    this->nrOfEntities--;
}

Entity* Scene::GetEntity(std::string entityName)
{
    if (this->EntityExists(entityName))
    {
        return this->entities[entityName];
    }

    std::cout << "No Entity with name: \"" << entityName << "\" was found." << std::endl;
    return nullptr;
}

std::map<std::string, Entity*>* Scene::GetEntities()
{
	return &this->entities;
}

unsigned int Scene::GetNrOfEntites()
{
    return this->nrOfEntities;
}

void Scene::UpdateEntities()
{
    for (auto const& [entityName, entity] : this->entities)
    {
        entity->Update();
    }
}

bool Scene::EntityExists(std::string entityName)
{
    for (auto const& [entityNameInScene, entity] : this->entities)
    {
        // An entity with this name already exists
        if (entityNameInScene == entityName)
        {
            return true;
        }
    }
    return false;
}
