#include "../structs.h"

float3 CalcDirLight(
	in DirectionalLight dirLight,
	in float3 camPos,
	in float3 fragPos,
	in float3 ambientMap,
	in float3 diffuseMap,
	in float3 specularMap,
	in float3 normalMap)
{
	// Ambient
	float3 ambient = ambientMap * dirLight.baseLight.ambient.rgb;

	// Diffuse
	float3 lightDir = normalize(-dirLight.direction.xyz);
	float alpha = max(dot(normalMap, lightDir), 0.0f);
	float3 diffuse = diffuseMap * alpha * dirLight.baseLight.diffuse.rgb;;

	// Specular
	float3 vecToCam = normalize(camPos - fragPos);
	float3 reflection = normalize(reflect(-lightDir.xyz, normalMap.xyz));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 100);
	float3 finalSpecular = specularMap.rgb * dirLight.baseLight.specular.rgb * spec;

	float3 DirLightContribution = (ambient.rgb + diffuse.rgb + finalSpecular.rgb);
	return DirLightContribution;
}

float3 CalcPointLight(
	in PointLight pointLight,
	in float3 camPos,
	in float3 fragPos,
	in float3 ambientMap,
	in float3 diffuseMap,
	in float3 specularMap,
	in float3 normalMap)
{
	float3 pointLightContribution = float3(0.0f, 0.0f, 0.0f);

	// Ambient
	float3 ambient = ambientMap * pointLight.baseLight.ambient.rgb;

	// Diffuse
	float3 lightDir = normalize(pointLight.position.xyz - fragPos);
	float alpha = max(dot(normalMap, lightDir), 0.0f);
	float3 diffuse = diffuseMap * alpha * pointLight.baseLight.diffuse.rgb;

	// Specular
	float3 vecToCam = normalize(camPos - fragPos);
	float3 reflection = normalize(reflect(-lightDir.xyz, normalMap.xyz));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 100);
	float3 finalSpecular = specularMap.rgb * pointLight.baseLight.specular.rgb * spec;

	// Attenuation
	float constantFactor = pointLight.attenuation.x;
	float linearFactor = pointLight.attenuation.y;
	float quadraticFactor = pointLight.attenuation.z;

	float distancePixelToLight = length(pointLight.position.xyz - fragPos);
	float attenuation = 1.0f / (constantFactor + (linearFactor * distancePixelToLight) + (quadraticFactor * pow(distancePixelToLight, 2)));

	pointLightContribution = float3(ambient.rgb + attenuation * (diffuse.rgb + finalSpecular.rgb));

	return pointLightContribution;
}

float3 CalcSpotLight(
	in SpotLight spotLight,
	in float3 camPos,
	in float3 fragPos,
	in float3 ambientMap,
	in float3 diffuseMap,
	in float3 specularMap,
	in float3 normalMap)
{
	float3 lightDir = normalize(spotLight.position_cutOff.xyz - fragPos);
	
	// Calculate the angle between lightdir and the direction of the light
	float theta = dot(lightDir, normalize(-spotLight.direction_outerCutoff.xyz));

	// To smooth edges
	float epsilon = (spotLight.position_cutOff.w - spotLight.direction_outerCutoff.w);
	float intensity = clamp((theta - spotLight.direction_outerCutoff.w) / epsilon, 0.0f, 1.0f);

	float3 spotLightContribution = float3(0.0f, 0.0f, 0.0f);

	// Ambient
	float3 ambient = ambientMap * spotLight.baseLight.ambient.rgb;

	// Diffuse
	float alpha = max(dot(normalMap, lightDir), 0.0f);
	float3 diffuse = diffuseMap * alpha * spotLight.baseLight.diffuse.rgb;

	// Specular
	float3 vecToCam = normalize(camPos - fragPos);
	float3 reflection = normalize(reflect(-lightDir.xyz, normalMap.xyz));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 100);
	float3 finalSpecular = specularMap.rgb * spotLight.baseLight.specular.rgb * spec;

	// Attenuation
	float constantFactor = spotLight.attenuation.x;
	float linearFactor = spotLight.attenuation.y;
	float quadraticFactor = spotLight.attenuation.z;

	float distancePixelToLight = length(spotLight.position_cutOff.xyz - fragPos);
	float attenuation = 1.0f / (constantFactor + (linearFactor * distancePixelToLight) + (quadraticFactor * pow(distancePixelToLight, 2)));
		
	spotLightContribution = float3(ambient.rgb + attenuation* (diffuse.rgb + finalSpecular.rgb)) * intensity;
	
	return spotLightContribution;
}

