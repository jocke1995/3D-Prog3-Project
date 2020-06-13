#include "RenderComponent.h"

RenderComponent::RenderComponent()
{
	this->transform = new Transform();
}

RenderComponent::~RenderComponent()
{
	delete this->transform;

	for (SlotInfo* slotInfo : this->info)
	{
		delete slotInfo;
	}
}

void RenderComponent::SetMeshes(std::vector<Mesh*> *meshes)
{
	this->meshes = *meshes;

	for (int i = 0; i < this->meshes.size(); i++)
	{
		this->info.push_back(new SlotInfo());
		this->info[i]->vertexDataIndex = this->meshes[i]->GetVertexDataIndex();
	}
}

void RenderComponent::SetDrawFlag(UINT drawFlag)
{
	this->drawFlag = drawFlag;
}

void RenderComponent::Update()
{
	this->transform->UpdateWorldMatrix();
}

Transform* RenderComponent::GetTransform()
{
	return this->transform;
}

Mesh* RenderComponent::GetMesh(unsigned int index)
{
	return this->meshes[index];
}

UINT RenderComponent::GetDrawFlag()
{
	return this->drawFlag;
}

unsigned int RenderComponent::GetNrOfMeshes()
{
	return this->meshes.size();
}

SlotInfo* RenderComponent::GetSlotInfo(unsigned int index)
{
	return this->info[index];
}
