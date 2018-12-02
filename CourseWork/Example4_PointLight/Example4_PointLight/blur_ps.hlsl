#define NEIGHBOUR_COUNT 20

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
    float screenDimension;
    float horScale;
    float verScale;
    float padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    
    float4 colour;
	// Initialize the colour to black.
    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float weight[NEIGHBOUR_COUNT];
	// Calculate weighting values
    weight[0] = 0.015f;
    float remainder = (1.0f - weight[0]);
    remainder /= 2.0f;

    for (int i = 1; i < NEIGHBOUR_COUNT; i++)
    {
        weight[i] = remainder / 2.0f;
        remainder -= weight[i];
    }

    weight[0] += remainder;


    float texelSize = 1.0f / screenDimension;
    // Add the vertical pixels to the colour by the specific weight of each.

    colour += shaderTexture.Sample(SampleType, input.tex) * weight[0];

    for (int j = 1; j < NEIGHBOUR_COUNT; j++)
    {
        colour += shaderTexture.Sample(SampleType, input.tex + float2(horScale * (texelSize * -j), verScale * (texelSize * -j))) * weight[j];
        colour += shaderTexture.Sample(SampleType, input.tex + float2(horScale * (texelSize * j), verScale * (texelSize * j))) * weight[j];
    }

	// Set the alpha channel to one.
    colour.a = 1.0f;

    return saturate(colour);
}