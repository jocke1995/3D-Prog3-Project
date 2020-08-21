#ifndef BOUNDINGBOXCOMPONENT_H
#define BOUNDINGBOXCOMPONENT_H

#include "Component.h"
#include "../Entity.h"

// Creating the BB out of the model
#include "MeshComponent.h"

// Using the same transform as the models transform
#include "TransformComponent.h"

namespace component
{
	class BoundingBoxComponent : public Component
	{
	public:
		BoundingBoxComponent(Entity* parent);
		virtual ~BoundingBoxComponent();

		void Init();
		void Update(double dt);

		// Renderer will call this when creating the bounding box
		void SetMesh(Mesh* mesh);

		// This will only be a valid call if the mesh is drawn. Otherwise use other functions to get the data needed.
		Mesh* GetMesh() const;

		Transform* GetTransform() const;
		const std::vector<Vertex>* GetVertices() const;
		const std::vector<unsigned int>* GetIndices() const;

	private:
		std::vector<Vertex> boundingBoxVertices;
		std::vector<unsigned int> boundingBoxIndices;

		// Will be set when scene is set to be drawn in renderer
		Mesh* mesh = nullptr;
		Transform* transform = nullptr;

		void CreateBoundingBox();
	};
}

#endif
