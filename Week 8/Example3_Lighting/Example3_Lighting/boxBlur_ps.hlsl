// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer ScreenBuffer : register(b0)
{
	float2 resolution;
	float2 padding;
}

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float2 pixelSize = 1.f / resolution;

	float4 colour = (0.f, 0.f, 0.f, 0.f);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			colour += texture0.Sample(Sampler0, input.tex + float2(pixelSize.x * i, pixelSize.y * j));
		}
	}

	colour /= 9.f;
	colour.a = 1.f;

	return colour;
}