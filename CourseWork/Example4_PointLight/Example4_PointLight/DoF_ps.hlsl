// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D depthTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D blurTexture : register(t2);
SamplerState Sampler0 : register(s0);

cbuffer DoFBuffer : register(b0)
{
    float Near;
    float Far;
    float Range;
    float dist;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 normalColor = normalTexture.Sample(Sampler0, input.tex);
    float4 blurColor = blurTexture.Sample(Sampler0, input.tex);
    float depthVal = depthTexture.Sample(Sampler0, input.tex).r;
    float Dist = depthTexture.Sample(Sampler0, float2(0.5f, 0.5f)).r;

    // Invert depth and distance values
    depthVal = 1.0f - depthVal;
    Dist = 1.0f - Dist;
	
	// Convert depth values to distance from near to far plane
    Dist *= (Far - Near);
    depthVal *= (Far - Near);

	// Get difference between current fragment and focal distance
    float diff = abs((depthVal - Dist) / Range);
    diff = saturate(diff);

    return lerp(normalColor, blurColor, diff);
}