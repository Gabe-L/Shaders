
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CamBuffer : register(b1)
{
	float4 camPos;
	float time;
	float3 padding;
}

cbuffer PositionBuffer
{
	static float3 g_positions[4] =
	{
		float3(-1, 1, 0),
		float3(-1, -1, 0),
		float3(1, 1, 0),
		float3(1, -1, 0)
	};
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
};

[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	// Append plane tri
	output.worldPosition = mul(input[0].position, worldMatrix).xyz;
	output.position = mul(input[0].position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex;
	output.normal = input[0].normal;
	triStream.Append(output);

	output.worldPosition = mul(input[1].position, worldMatrix).xyz;
	output.position = mul(input[1].position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[1].tex;
	output.normal = input[1].normal;
	triStream.Append(output);

	output.worldPosition = mul(input[2].position, worldMatrix).xyz;
	output.position = mul(input[2].position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[2].tex;
	output.normal = input[2].normal;
	triStream.Append(output);

	triStream.RestartStrip();
}