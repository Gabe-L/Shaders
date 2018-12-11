SamplerState Sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

// Textures
Texture2D texture0 : register(t0);
Texture2D grassTex : register(t1);
Texture2D spotLightShadow : register(t2);
Texture2D explopsionShadows[6] : register(t3);

// Light info : 0 is explosion, 1 is spot light
cbuffer LightBuffer : register(b0)
{
    float4 ambient[2];
    float4 diffuse[2];
    float4 direction[2];
    float4 position[2];
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 worldPosition : TEXCOORD1;
    float4 depthPosition : TEXCOORD2;
    float4 spotViewPos : TEXCOORD3;
    float4 explosionViewPos[6] : TEXCOORD4;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(ldiffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_Target
{
    float depthValue;
    float lightDepthValue;
	float shadowMapBias = 0.0005f;// 0.0001f;
    float4 colour = float4(0.f, 0.f, 0.f, 1.f);
    float4 textureColour;
    float2 pTexCoords;

    if (input.worldPosition.w == 0.0f)
    {
        textureColour = grassTex.Sample(Sampler0, input.tex);
    }
    else
    {
        textureColour = texture0.Sample(Sampler0, input.tex);
    }

    // Exlplosion (point) light
    
    if (length(diffuse[0]) > 0.1f)
    {
    
        for (int i = 0; i < 6; i++)
        {
            pTexCoords = input.explosionViewPos[i].xy / input.explosionViewPos[i].w;

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
                    float3 lightVector = (position[0].xyz - input.worldPosition.xyz);
                    float dist = length(lightVector);
                    float attenuation = 1 / (1.0f + (0.075f * dist));

                    colour += calculateLighting(lightVector, input.normal, diffuse[0]) * attenuation;
				    // Break out so multiple light values aren't given by one point light
                    break;
                }
            }
        }
    }
    // Spot light
    pTexCoords = input.spotViewPos.xy / input.spotViewPos.w;
    pTexCoords *= float2(0.5, -0.5);
    pTexCoords += float2(0.5f, 0.5f);

    float2 centre = float2(0.5f, 0.5f);
    float dist = pow(pTexCoords.x - centre.x, 2) + pow(pTexCoords.y - centre.y, 2);
    dist = sqrt(dist);

    if (dist < 0.4f)
    {
		// Sample the shadow map (get depth of geometry)
        depthValue = spotLightShadow.Sample(shadowSampler, pTexCoords).r;

		// Calculate the depth from the light.
        lightDepthValue = input.spotViewPos.z / input.spotViewPos.w;
        lightDepthValue -= shadowMapBias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        if (lightDepthValue < depthValue)
        {
            float dist = length(position[1].xyz - input.worldPosition.xyz);
			float attenuation = 1 / (1.0f + (0.025f * dist));// +(0.0025 * pow(dist, 2)));
            colour += calculateLighting(-direction[1].xyz, input.normal, diffuse[1]) * attenuation;
        }
}

	colour = saturate(colour + ambient[0]);
	return colour * textureColour;

}