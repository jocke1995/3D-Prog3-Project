#include "../structs.h"

Texture2D textures[]   : register (t0);
SamplerState samplerTypeWrap	: register (s0);
SamplerState samplerTypeClamp	: register (s1);

float CalculateShadow(
	in float4 fragPosLightSpace,
	in float shadowMapIndex,
	in float3 normal,
	in float3 lightDir)
{
	// Perform perspective divide
	float2 texCoord = fragPosLightSpace.xy / fragPosLightSpace.w;

	// Transform to [0,1] range
	texCoord = texCoord * 0.5 + 0.5;
	texCoord.y = 1 - texCoord.y;

	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepthFromLight = textures[shadowMapIndex].Sample(samplerTypeClamp, texCoord).r;

	// get depth of current fragment from light's perspective
	float depthFromLightToFragPos = fragPosLightSpace.z / fragPosLightSpace.w;

	// check whether current fragPos is in shadow
	float shadow = 0.0f;
	//float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.00005);	// LearnOpenGl Stuff
	float bias = 0.0005f;
	if (depthFromLightToFragPos - bias > closestDepthFromLight)
	{
		// the pixel is in shadow
		shadow = 1.0f;
	}

	return shadow;
}

float3 CalcDirLight(
	in DirectionalLight dirLight,
	in float3 camPos,
	in float4 fragPos,
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
	float3 vecToCam = normalize(camPos - fragPos.xyz);
	float3 reflection = normalize(reflect(-lightDir.xyz, normalMap.xyz));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 100);
	float3 specular = specularMap.rgb * dirLight.baseLight.specular.rgb * spec;

	float shadow = 0.0f;
	if (dirLight.baseLight.castShadow == true)
	{
		float4 fragPosLightSpace = mul(fragPos, dirLight.viewProj);

		shadow = CalculateShadow(fragPosLightSpace, dirLight.textureShadowMap, normalMap, lightDir);
	}

	float3 DirLightContribution = ambient.rgb + ((1.0f - shadow) * (diffuse.rgb + specular.rgb));
	return DirLightContribution;
}

float3 CalcPointLight(
	in PointLight pointLight,
	in float3 camPos,
	in float4 fragPos,
	in float3 ambientMap,
	in float3 diffuseMap,
	in float3 specularMap,
	in float3 normalMap)
{
	float3 pointLightContribution = float3(0.0f, 0.0f, 0.0f);

	// Ambient
	float3 ambient = ambientMap * pointLight.baseLight.ambient.rgb;

	// Diffuse
	float3 lightDir = normalize(pointLight.position.xyz - fragPos.xyz);
	float alpha = max(dot(normalMap, lightDir), 0.0f);
	float3 diffuse = diffuseMap * alpha * pointLight.baseLight.diffuse.rgb;

	// Specular
	float3 vecToCam = normalize(camPos - fragPos.xyz);
	float3 reflection = normalize(reflect(-lightDir.xyz, normalMap.xyz));
	float spec = pow(max(dot(reflection, vecToCam), 0.0), 100);
	float3 specular = specularMap.rgb * pointLight.baseLight.specular.rgb * spec;

	// Attenuation
	float constantFactor = pointLight.attenuation.x;
	float linearFactor = pointLight.attenuation.y;
	float quadraticFactor = pointLight.attenuation.z;

	float distancePixelToLight = length(pointLight.position.xyz - fragPos.xyz);
	float attenuation = 1.0f / (constantFactor + (linearFactor * distancePixelToLight) + (quadraticFactor * pow(distancePixelToLight, 2)));

	pointLightContribution = float3(ambient.rgb + attenuation * (diffuse.rgb + specular.rgb));

	return pointLightContribution;
}

float3 CalcSpotLight(
	in SpotLight spotLight,
	in float3 camPos,
	in float4 fragPos,
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
	float3 specular = specularMap.rgb * spotLight.baseLight.specular.rgb * spec;

	// Attenuation
	float constantFactor = spotLight.attenuation.x;
	float linearFactor = spotLight.attenuation.y;
	float quadraticFactor = spotLight.attenuation.z;

	float distancePixelToLight = length(spotLight.position_cutOff.xyz - fragPos);
	float attenuation = 1.0f / (constantFactor + (linearFactor * distancePixelToLight) + (quadraticFactor * pow(distancePixelToLight, 2)));
		
	float shadow = 0.0f;
	if (spotLight.baseLight.castShadow == true)
	{
		float4 fragPosLightSpace = mul(fragPos, spotLight.viewProj);

		shadow = CalculateShadow(fragPosLightSpace, spotLight.textureShadowMap, normalMap, lightDir);
	}

	spotLightContribution =  float3(ambient.rgb + (1.0f - shadow) * attenuation* (diffuse.rgb + specular.rgb)) * intensity;
	
	return spotLightContribution;
}
