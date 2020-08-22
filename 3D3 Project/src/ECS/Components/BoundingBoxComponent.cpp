#include "BoundingBoxComponent.h"

namespace component
{
	BoundingBoxComponent::BoundingBoxComponent(Entity* parent)
		:Component(parent)
	{
		
	}

	BoundingBoxComponent::~BoundingBoxComponent()
	{
		delete this->mesh;
	}

	void BoundingBoxComponent::Init()
	{
		this->CreateBoundingBox();
	}

	void BoundingBoxComponent::Update(double dt)
	{

	}

	void BoundingBoxComponent::SetMesh(Mesh* mesh)
	{
		this->mesh = mesh;
	}

	Mesh* BoundingBoxComponent::GetMesh() const
	{
		return this->mesh;
	}

	Transform* BoundingBoxComponent::GetTransform() const
	{
		return this->transform;
	}

	const std::vector<Vertex>* BoundingBoxComponent::GetVertices() const
	{
		return &this->boundingBoxVertices;
	}

	const std::vector<unsigned int>* BoundingBoxComponent::GetIndices() const
	{
		return &this->boundingBoxIndices;
	}

	bool BoundingBoxComponent::CreateBoundingBox()
	{
		if (this->parent->HasComponent<MeshComponent>() == true && this->parent->HasComponent<TransformComponent>() == true)
		{
			// Use the same transform as the model
			this->transform = this->parent->GetComponent<TransformComponent>()->GetTransform();

			// Create new mesh (the bounding box)
			float3 minVertex = { 1000000.0f, 1000000.0f, 1000000.0f };
			float3 maxVertex = { -1000000.0f, -1000000.0f, -1000000.0f };

			for (unsigned int i = 0; i < this->parent->GetComponent<MeshComponent>()->GetNrOfMeshes(); i++)
			{
				std::vector<Vertex> modelVertices = *this->parent->GetComponent<MeshComponent>()->GetMesh(i)->GetVertices();
				for (unsigned int j = 0; j < modelVertices.size(); j++)
				{
					minVertex.x = Min(minVertex.x, modelVertices[j].pos.x);
					minVertex.y = Min(minVertex.y, modelVertices[j].pos.y);
					minVertex.z = Min(minVertex.z, modelVertices[j].pos.z);

					maxVertex.x = Max(maxVertex.x, modelVertices[j].pos.x);
					maxVertex.y = Max(maxVertex.y, modelVertices[j].pos.y);
					maxVertex.z = Max(maxVertex.z, modelVertices[j].pos.z);
				}
			}

			// Create bounding box
			Vertex v[8] = {};

			// Front vertices
			v[0].pos = { minVertex.x, minVertex.y, minVertex.z, 1.0f};
			v[1].pos = { minVertex.x, maxVertex.y, minVertex.z, 1.0f };
			v[2].pos = { maxVertex.x, maxVertex.y, minVertex.z, 1.0f };
			v[3].pos = { maxVertex.x, minVertex.y, minVertex.z, 1.0f };

			// Back vertices
			v[4].pos = { minVertex.x, minVertex.y, maxVertex.z, 1.0f };
			v[5].pos = { minVertex.x, maxVertex.y, maxVertex.z, 1.0f };
			v[6].pos = { maxVertex.x, maxVertex.y, maxVertex.z, 1.0f };
			v[7].pos = { maxVertex.x, minVertex.y, maxVertex.z, 1.0f };

			for (unsigned int i = 0; i < 8; i++)
			{
				this->boundingBoxVertices.push_back(v[i]);
			}

			// Indices
			unsigned int indices[36] = {};
			// Front Face
			indices[0] = 0; indices[1] = 1; indices[2] = 3;
			indices[3] = 1; indices[4] = 2; indices[5] = 3;
			
			// Back Face
			indices[6] = 6; indices[7] = 5; indices[8] = 4;
			indices[9] = 7; indices[10] = 6; indices[11] = 4;
			
			// Top Face
			indices[12] = 5; indices[13] = 6; indices[14] = 1;
			indices[15] = 1; indices[16] = 6; indices[17] = 2;
			
			// Bottom Face
			indices[18] = 3; indices[19] = 4; indices[20] = 0;
			indices[21] = 3; indices[22] = 7; indices[23] = 4;
			
			// Right Face
			indices[24] = 4; indices[25] = 5; indices[26] = 0;
			indices[27] = 5; indices[28] = 1; indices[29] = 0;
			
			// Left Face
			indices[30] = 3; indices[31] = 2; indices[32] = 7;
			indices[33] = 2; indices[34] = 6; indices[35] = 7;

			for (unsigned int i = 0; i < 36; i++)
			{
				this->boundingBoxIndices.push_back(indices[i]);
			}

			return true;
		}
		else
		{
			Log::PrintSeverity(Log::Severity::CRITICAL, "Trying to add a bounding box when no mesh and/or transform exists on entity.\n");
			return false;
		}
	}
}
