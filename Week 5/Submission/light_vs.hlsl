// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer TimeBuffer : register(b1)
{
	float time;
	float height;
	float freq;
	float speed;
}

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
	float timeInfo : TEXCOORD2;
};

OutputType main(InputType input)
{
	OutputType output;

	////offset position based on sine wave
	//input.position.y += height * length(input.normal) * (sin((input.position.x * freq) + (time * speed)));
	//input.position.z += height * length(input.normal) * (sin((input.position.y * freq) + (time * speed)));
	//input.position.x += height * length(input.normal) * (sin((input.position.z * freq) + (time * speed)));


	////modify normals
	//input.normal.x = 1 - cos(input.position.x + time);
	//input.normal.y = abs(cos(input.position.x + time));


	//float4 textureColour = texture0.Sample(sampler0, input.tex);
	float4 colour = texture0.SampleLevel(sampler0, input.tex, 0);
	input.position.y = 10 * colour.r;


	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	output.worldPosition = mul(input.position, worldMatrix).xyz;

	output.timeInfo = time;

	return output;
}