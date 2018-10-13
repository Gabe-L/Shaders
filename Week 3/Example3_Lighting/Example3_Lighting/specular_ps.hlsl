// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

//FOLLOWING SOURCES USED:
//http://www.codinglabs.net/article_physically_based_rendering_cook_torrance.aspx
//https://github.com/glslify/glsl-specular-cook-torrance/blob/master/index.glsl
//https://github.com/thefranke/dirtchamber/blob/master/shader/brdf.hlsl
#define F0_IRON         0.56,0.57,0.58

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float specularPower;
	float4 specularColour;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewVector : TEXCOORD1;
	float4 testVars : TEXCOORD2;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	colour += ambientColour;
	return saturate(colour);
}

float dist(float NdotH, float alpha)
{
	float a2 = alpha * alpha;
	float cos2 = NdotH * NdotH;

	return (1.0f / 3.141592654f) * pow(alpha / (cos2 * (a2 - 1) + 1), 2);

}

float geo(float NdotH, float VdotH, float VdotN, float LdotN)
{
	float x = saturate(2.0 * NdotH / VdotH);
	return min(1.f, min(x * VdotN, x * LdotN));
}

float3 frs(float VdotN, float refractiveIndex, float metallic, float4 diffuseColour)
{
	/*float f0 = pow((1 - refractiveIndex) / (1 + refractiveIndex), 2);

	return (f0 + (1.0f - f0) * pow(1.0f - LdotH, metallic));*/

	float3 F0 = abs((1.0 - refractiveIndex) / (1.0 + refractiveIndex));
	F0 = F0 * F0;
	F0 = lerp(F0, diffuseColour.rgb, metallic);

	return F0 + (1 - F0) * pow(1 - VdotN, 5);

}

float cookTor(float3 lightDirection, float3 viewVec, float3 surfaceNormal, float roughness, float refractiveIndex, float metallic, float4 diffuseColour) {

	float alpha = roughness * roughness;

	//View Direction From View Vector
	float3 viewDirection = normalize(viewVec);
	
	//Half angle vector
	float3 halfway = normalize(lightDirection + viewDirection);

	//dot products
	float VdotN = saturate(dot(viewDirection, surfaceNormal));
	float LdotN = saturate(dot(lightDirection, surfaceNormal));
	float LdotH = saturate(dot(lightDirection, halfway));
	float NdotH = saturate(dot(surfaceNormal, halfway));
	float VdotH = saturate(dot(viewDirection, halfway));

	//Geometric term
	float G = geo(NdotH, VdotH, VdotN, LdotN);

	//Distribution term
	float D = dist(NdotH, alpha);

	//fresnel term
	float F = pow(1.0 - VdotN, frs(VdotN, refractiveIndex, metallic, diffuseColour));

	//Multiply terms and done
	return  G * F * D / 4 * (max(3.14159265 * VdotN * LdotN, 0.000001));
}

float4 calcSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4
	specularColour, float specularPower)
{
	// blinn-phong specular calculation
	float3 halfway = normalize(lightDirection + viewVector);
	float NoH = saturate(dot(normal, halfway));
	float specularIntensity = pow(max(NoH, 0.0), specularPower);
	return saturate(specularColour * specularIntensity);
}

float4 main(InputType input) : SV_TARGET
{

	float4 textureColour;
	float4 lightColour;


	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(sampler0, input.tex);
	lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
	
	//float4 spec = calcSpecular(-lightDirection, input.normal, input.viewVector, specularColour, 64);

	float4 spec = cookTor(-lightDirection, input.viewVector, input.normal, input.testVars.x, input.testVars.y, input.testVars.z, diffuseColour);

	lightColour += spec;

	return (saturate(lightColour /** textureColour*/));

}