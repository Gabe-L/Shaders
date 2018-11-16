
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D depthMapTexture2 : register(t2);

SamplerState diffuseSampler  : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambient[2];
	float4 diffuse[2];
	float4 direction[2];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPos : TEXCOORD1;
    float4 lightViewPos2 : TEXCOORD2;
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

	// Calculate the projected texture coordinates.
    float2 pTexCoord = input.lightViewPos.xy / input.lightViewPos.w;
    pTexCoord *= float2(0.5, -0.5);
    pTexCoord += float2(0.5f, 0.5f);

    // Calculate the projected texture coordinates.
    float2 pTexCoord2 = input.lightViewPos2.xy / input.lightViewPos2.w;
    pTexCoord2 *= float2(0.5, -0.5);
    pTexCoord2 += float2(0.5f, 0.5f);


    // Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    if (!((pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f) || (pTexCoord2.x < 0.f || pTexCoord2.x > 1.f || pTexCoord2.y < 0.f || pTexCoord2.y > 1.f)))
    {
        // Sample the shadow map (get depth of geometry)
        depthValue = depthMapTexture.Sample(shadowSampler, pTexCoord).r;
	    // Calculate the depth from the light.
        lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
        lightDepthValue -= shadowMapBias;

	    // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        if (lightDepthValue < depthValue)
        {
            colour += calculateLighting(-direction[0].xyz, input.normal, diffuse[0]);
        }
        lit = 1;
    }

    // Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    if (!(pTexCoord2.x < 0.f || pTexCoord2.x > 1.f || pTexCoord2.y < 0.f || pTexCoord2.y > 1.f))
    {
        // Sample the shadow map (get depth of geometry)
        depthValue = depthMapTexture2.Sample(shadowSampler, pTexCoord2).r;
	    // Calculate the depth from the light.
        lightDepthValue = input.lightViewPos2.z / input.lightViewPos2.w;
        lightDepthValue -= shadowMapBias;

	    // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        if (lightDepthValue < depthValue)
        {
            colour += calculateLighting(-direction[1].xyz, input.normal, diffuse[1]);
        }

        lit = 1;
    }

    if (lit == 0)
    {
        return textureColour;
    }
    else
    {
        colour = saturate(colour + ambient[0]);
        return saturate(colour) * textureColour;
    }
}