#include "RenderComponent.h"

RenderComponent::RenderComponent()
{
	this->transform = new Transform();
}

RenderComponent::~RenderComponent()
{
	delete this->transform;
}

void RenderComponent::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
	this->info.vertexDataIndex = mesh->GetVertexDataIndex();
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

Mesh* RenderComponent::GetMesh()
{
	return this->mesh;
}

UINT RenderComponent::GetDrawFlag()
{
	return this->drawFlag;
}

SlotInfo* RenderComponent::GetSlotInfo()
{
	return &this->info;
}