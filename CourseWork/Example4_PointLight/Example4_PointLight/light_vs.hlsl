// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
    matrix projectionMatrix;
    matrix spotProjectionMatrix;
    matrix spotViewMatrix;
	matrix explosionProjectionMatrix;
    matrix explosionViewMatrices[6];
};

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
    float4 spotViewPos : TEXCOORD2;
    float4 explosionViewPos[6] : TEXCOORD3;
};

OutputType main(InputType input)
{
	OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.worldPosition = mul(input.position, worldMatrix).xyz;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

    output.spotViewPos = mul(input.position, worldMatrix);
    output.spotViewPos = mul(output.spotViewPos, spotViewMatrix);
    output.spotViewPos = mul(output.spotViewPos, spotProjectionMatrix);

	for (int i = 0; i < 6; i++) {
		output.explosionViewPos[i] = mul(input.position, worldMatrix);
		output.explosionViewPos[i] = mul(output.explosionViewPos[i], explosionViewMatrices[i]);
		output.explosionViewPos[i] = mul(output.explosionViewPos[i], explosionProjectionMatrix);
	}

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);


	return output;
}