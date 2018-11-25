// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
Texture2D depthTex : register(t1);

SamplerState sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambient;
	float4 diffuse;
    float4 direction;
	float3 position;
	float padding;
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
    float4 lightViewPos : TEXCOORD2;
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
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	//float4 textureColour = texture0.Sample(sampler0, input.tex);
	//float3 lightVector = normalize(position - input.worldPosition);
	//float4 lightColour = ambient + calculateLighting(lightVector, input.normal, diffuse);
	
	//return lightColour * textureColour;

    float depthValue;
    float lightDepthValue;
    float shadowMapBias = 0.005f;
    float4 colour = float4(0.f, 0.f, 0.f, 1.f);
    float4 textureColour = texture0.Sample(sampler0, input.tex);
    int lit = 0;

    float2 pTexCoords = input.lightViewPos.xy / input.lightViewPos.w;
    pTexCoords *= float2(0.5, -0.5);
    pTexCoords += float2(0.5f, 0.5f);

		// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    if (!(pTexCoords.x < 0.f || pTexCoords.x > 1.f || pTexCoords.y < 0.f || pTexCoords.y > 1.f))
    {
		// Sample the shadow map (get depth of geometry)
        depthValue = depthTex.Sample(shadowSampler, pTexCoords).r;

			// Calculate the depth from the light.
        lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
        lightDepthValue -= shadowMapBias;

			// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        if (lightDepthValue < depthValue)
        {
            colour += calculateLighting(-direction.xyz, input.normal, diffuse[2]);
				// Break out so multiple light values aren't given by one point light
            lit = 1;
        }

    }

    if (lit == 0)
    {
        return ambient[0] * textureColour;
    }
    else
    {
        colour = saturate(colour + ambient[0]);
        return colour * textureColour;
    }

}



