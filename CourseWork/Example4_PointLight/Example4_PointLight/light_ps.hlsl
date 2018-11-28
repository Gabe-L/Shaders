// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
Texture2D explopsionShadows[6] : register (t1);

SamplerState sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambient;
	float4 diffuse;
    float4 direction;
	float4 position;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
	float4 explosionViewPos[6] : TEXCOORD2;
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
    float depthValue;
    float lightDepthValue;
    float shadowMapBias = 0.001f;
    float4 colour = float4(0.f, 0.f, 0.f, 1.f);
    float4 textureColour = texture0.Sample(sampler0, input.tex);
    int lit = 0;

	for (int i = 0; i < 6; i++)
	{
		float2 pTexCoords = input.explosionViewPos[i].xy / input.explosionViewPos[i].w;
		pTexCoords *= float2(0.5, -0.5);
		pTexCoords += float2(0.5f, 0.5f);

		// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
		if (!(pTexCoords.x < 0.f || pTexCoords.x > 1.f || pTexCoords.y < 0.f || pTexCoords.y > 1.f))
		{
			// Sample the shadow map (get depth of geometry)
			depthValue = explopsionShadows[i].Sample(shadowSampler, pTexCoords).r;

			// Calculate the depth from the light.
			lightDepthValue = input.explosionViewPos[i].z / input.explosionViewPos[i].w;
			lightDepthValue -= shadowMapBias;

			// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
			if (lightDepthValue < depthValue)
			{
				float3 lightVector = normalize(position.xyz - input.worldPosition);
				colour += calculateLighting(lightVector, input.normal, diffuse);
				// Break out so multiple light values aren't given by one point light
				lit = 1;
				break;
			}

		}
	}

    if (lit == 0)
    {
        return ambient * textureColour;
    }
    else
    {
        colour = saturate(colour + ambient);
        return colour * textureColour;
    }

}



