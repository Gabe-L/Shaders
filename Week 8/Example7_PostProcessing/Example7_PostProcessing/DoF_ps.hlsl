// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D depthTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D blurTexture : register(t2);
SamplerState Sampler0 : register(s0);

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
    depthVal = 1.0f - depthVal;


    // Hard coded values for now

    float Near = 1.0f;
    float Far = 100.0f / 99.0f;
    float Range = 5.0f;

    //--------------------------

    float Dz = -Near * Far;
    Dz /= depthVal - Far;

    float blurVal = saturate(abs(Dz - depthVal) / Range);

    float4 returnColor = lerp(normalColor, blurColor, blurVal);

    return returnColor;
}