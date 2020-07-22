#include "MeshComponent.h"

namespace component
{
	MeshComponent::MeshComponent(Entity* parent)
		:Component(parent)
	{

	}

	MeshComponent::~MeshComponent()
	{

	}

	void MeshComponent::SetMeshes(std::vector<Mesh*>* meshes)
	{
		this->meshes = *meshes;
	}

	void MeshComponent::SetDrawFlag(UINT drawFlag)
	{
		this->drawFlag = drawFlag;
	}

	void MeshComponent::Update()
	{

	}

	Mesh* MeshComponent::GetMesh(unsigned int index) const
	{
		return this->meshes[index];
	}

	UINT MeshComponent::GetDrawFlag() const
	{
		return this->drawFlag;
	}

	unsigned int MeshComponent::GetNrOfMeshes() const
	{
		return this->meshes.size();
	}
}
