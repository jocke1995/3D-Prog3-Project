#ifndef COPYLIGHTSTASK_H
#define COPYLIGHTSTASK_H

#include "CopyTask.h"

#include "ConstantBufferDefault.h"
#include "../ECS/Components/Lights/DirectionalLightComponent.h"
#include "../ECS/Components/Lights/PointLightComponent.h"
#include "../ECS/Components/Lights/SpotLightComponent.h"

class CopyLightsTask : public CopyTask
{
public:
	CopyLightsTask(
		ID3D12Device5* device,
		std::vector<std::pair<component::DirectionalLightComponent*, ConstantBufferDefault*>>* directionalLights,
		std::vector<std::pair<component::PointLightComponent*, ConstantBufferDefault*>>* pointLights,
		std::vector<std::pair<component::SpotLightComponent*, ConstantBufferDefault*>>* spotLights);
	virtual ~CopyLightsTask();

	void Execute();
private:
	std::vector<std::pair<component::DirectionalLightComponent*, ConstantBufferDefault*>>* directionalLights = nullptr;
	std::vector<std::pair<component::PointLightComponent*, ConstantBufferDefault*>>* pointLights = nullptr;
	std::vector<std::pair<component::SpotLightComponent*, ConstantBufferDefault*>>* spotLights = nullptr;

	void CopyResource(
		ID3D12GraphicsCommandList5* commandList,
		Resource* uploadResource, Resource* defaultResource,
		void* data);
};

#endif