// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambient[4];
	float4 diffuse[4];
	float4 position[2];
	float4 direction[2];
};

cbuffer AttenuationBuffer : register(b1)
{
	float constant;
	float lin;
	float quadratic;
	float padding;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(ldiffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	float con_factor = constant;
	float lin_factor = lin;
	float quad_factor = quadratic;
	
	float4 lightColour = 0.f;
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	float4 textureColour = texture0.Sample(sampler0, input.tex);

	for (int i = 0; i < 2; i++) {
		//point lights
		float3 lightVector = position[i] - input.worldPosition;
		float dist = length(lightVector);
		float attenuation = 1 / (con_factor + (lin_factor * dist) + (quad_factor * pow(dist, 2)));

		lightVector = normalize(lightVector);
		lightColour += (calculateLighting(lightVector, input.normal, diffuse[i]) * attenuation);

		//directional lights
		lightColour += calculateLighting(-direction[i].xyz, input.normal, diffuse[i + 2]);

	}

	lightColour += ambient[0];

	return saturate(lightColour) *textureColour;
}



