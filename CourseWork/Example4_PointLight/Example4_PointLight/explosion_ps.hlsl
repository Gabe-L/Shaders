// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float2 disp : TEXCOORD2;
    float progress : TEXCOORD3;
};

float4 main(InputType input) : SV_TARGET
{
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    float4 textureColour = texture0.Sample(sampler0, input.disp + 0.15f);
    
    float prog = max(1.0f, input.progress);
    prog -= 1.0f;

    // Clip vertices based on progress through explosion
    float clipVal = 0.2f + (0.3f * (1.0f - prog));
    clip(clipVal - (input.disp.x + 0.15f));

    return textureColour;
}