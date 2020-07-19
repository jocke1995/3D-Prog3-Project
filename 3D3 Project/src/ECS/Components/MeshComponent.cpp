#include "MeshComponent.h"

namespace component
{
	MeshComponent::MeshComponent(Entity* parent)
		:Component(parent)
	{

	}

	MeshComponent::~MeshComponent()
	{
		for (SlotInfo* slotInfo : this->info)
		{
			delete slotInfo;
		}
	}

	void MeshComponent::SetMeshes(std::vector<Mesh*>* meshes)
	{
		this->meshes = *meshes;

		for (int i = 0; i < this->meshes.size(); i++)
		{
			this->info.push_back(new SlotInfo());
			this->info[i]->vertexDataIndex = this->meshes[i]->GetDescriptorHeapIndex();
		}
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

	SlotInfo* MeshComponent::GetSlotInfo(unsigned int index) const
	{
		return this->info[index];
	}
}
