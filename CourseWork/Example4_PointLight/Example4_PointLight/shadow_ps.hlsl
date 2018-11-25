
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState diffuseSampler : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 direction;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPos : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{
    float depthValue;
    float lightDepthValue;
    float shadowMapBias = 0.005f;
    float4 colour = float4(0.f, 0.f, 0.f, 1.f);
    float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);
    int lit = 0;

	// Point

    float2 pTexCoords = input.lightViewPos.xy / input.lightViewPos.w;
    pTexCoords *= float2(0.5, -0.5);
    pTexCoords += float2(0.5f, 0.5f);

	// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    if (!(pTexCoords.x < 0.f || pTexCoords.x > 1.f || pTexCoords.y < 0.f || pTexCoords.y > 1.f))
    {
		// Sample the shadow map (get depth of geometry)
		//depthValue = depthArray.Sample(shadowSampler, float3(pTexCoords, i)).r;
        depthValue = depthMapTexture.Sample(shadowSampler, pTexCoords).r;

		// Calculate the depth from the light.
        lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
        lightDepthValue -= shadowMapBias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        if (lightDepthValue < depthValue)
        {
            colour += calculateLighting(-direction.xyz, input.normal, diffuse);
			// Break out so multiple light values aren't given by one point light
            lit = 1;
        }

    }

    if (lit == 0)
    {
        return ambient * textureColour;
    }
    else
    {
        colour = saturate(colour + ambient[0]);
        return colour * textureColour;
    }
}