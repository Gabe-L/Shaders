// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	float4 pixelColour;

	textureColour = texture0.Sample(sampler0, input.tex);

	float grey = (textureColour.r + textureColour.g + textureColour.b) / 3;

	pixelColour = float4(grey, grey, grey, 1.0f);

	return pixelColour;
}
