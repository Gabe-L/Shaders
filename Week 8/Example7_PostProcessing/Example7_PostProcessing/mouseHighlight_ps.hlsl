// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer MouseBuffer : register(b0)
{
    float2 mousePos;
    float2 resolution;
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
    float4 textureColor = texture0.Sample(Sampler0, input.tex);

    float dist = distance(mousePos, (input.tex * resolution));

    float inRange = 50 - dist; // In range check - distance to point
    float fuzz = inRange / 50; // Decimal percentage of how close point is
    
    inRange = max(0, inRange);
    inRange = min(1, inRange);

    fuzz -= 0.5f;
    fuzz = max(0, fuzz);

    fuzz *= inRange;

    float color = 0.0f;
    color += textureColor.r;
    color += textureColor.g;
    color += textureColor.b;

    color /= 3;

    float4 grey = float4(color, color, color, 1.f);
    float4 finalColour;
    finalColour = (textureColor * inRange); // Negate natural color if out of range
    finalColour += (1 - inRange) * grey; // Negate greyscale if within range

    float4 fuzzColour;
    fuzzColour = (textureColor * fuzz); // Negate natural color if out of range
    fuzzColour += (0.5f - fuzz) * grey; // Negate greyscale if within range

   
    return lerp(finalColour, fuzzColour, 0.5f);
}