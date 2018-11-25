
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D depthMapTexture2 : register(t2);
Texture2D depthArray[6] : register(t3);

SamplerState diffuseSampler  : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambient[3];
	float4 diffuse[3];
	float4 direction[3];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPos : TEXCOORD1;
    float4 lightViewPos2 : TEXCOORD2;
	float4 lightViews[6] : TEXCOORD3;
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

    float low = 0.01f;

    float3 directions[6] =
    {
        float3(0.0f, 1, low), // Up
		float3(0.0f, -1, low), // Down
		float3(1, -0.33f, 0), // Right
		float3(-1, -0.33f, 0), // Left
		float3(0, -0.33f, 1), // Fowards
		float3(0, -0.33f, -1) // Backwards
    };

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

	// Directional

    // Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    //if (!(pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f))
    //{
    //    // Sample the shadow map (get depth of geometry)
    //    depthValue = depthMapTexture.Sample(shadowSampler, pTexCoord).r;
	   // // Calculate the depth from the light.
    //    lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
    //    lightDepthValue -= shadowMapBias;
    //    
	   // // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    //    if (lightDepthValue < depthValue)
    //    {
    //        //colour += calculateLighting(-direction[0].xyz, input.normal, diffuse[0]);
    //    }
    //    lit = 1;
    //}

	// Spot

	//float2 centre = float2(0.5f, 0.5f);

	//float dist = pow(pTexCoord2.x - centre.x, 2) + pow(pTexCoord2.y - centre.y, 2);
	//dist = sqrt(dist);

	//if (dist > 0.4f)
	//{
	//	return ambient[0] * textureColour;
	//}

    // Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    //if (!(pTexCoord2.x < 0.f || pTexCoord2.x > 1.f || pTexCoord2.y < 0.f || pTexCoord2.y > 1.f))
    //{
    //    // Sample the shadow map (get depth of geometry)
    //    depthValue = depthMapTexture2.Sample(shadowSampler, pTexCoord2).r;
	   // // Calculate the depth from the light.
    //    lightDepthValue = input.lightViewPos2.z / input.lightViewPos2.w;
    //    lightDepthValue -= shadowMapBias;

	   // // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    //    if (lightDepthValue < depthValue)
    //    {
    //        colour += calculateLighting(-direction[1].xyz, input.normal, diffuse[1]);
    //    }

    //    lit = 1;
    //}

	// Point

	for (int i = 0; i < 6; i++)
	{

		float2 pTexCoords = input.lightViews[i].xy / input.lightViews[i].w;
		pTexCoords *= float2(0.5, -0.5);
		pTexCoords += float2(0.5f, 0.5f);

		// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
		if (!(pTexCoords.x < 0.f || pTexCoords.x > 1.f || pTexCoords.y < 0.f || pTexCoords.y > 1.f))
		{
			// Sample the shadow map (get depth of geometry)
			//depthValue = depthArray.Sample(shadowSampler, float3(pTexCoords, i)).r;
			depthValue = depthArray[i].Sample(shadowSampler, pTexCoords).r;

			// Calculate the depth from the light.
			lightDepthValue = input.lightViews[i].z / input.lightViews[i].w;
			lightDepthValue -= shadowMapBias;

			// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
			if (lightDepthValue < depthValue)
			{
				colour += calculateLighting(-directions[i].xyz, input.normal, diffuse[2]);
				// Break out so multiple light values aren't given by one point light
				lit = 1;
			}

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