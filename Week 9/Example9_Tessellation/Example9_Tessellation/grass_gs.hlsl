
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
		float3(0, 0.5f, 0),
		float3(0, 0.5f, 0),
		float3(1, 0.5f, 0),
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
	float4 worldPosition : TEXCOORD1;
};

[maxvertexcount(6)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	// Make blade of grass

	float3 forward = normalize(camPos.xyz - input[0].position.xyz);
	float3 up = float3(0, 1, 0);
	float3 right = cross(forward, up);
	up = cross(right, forward);

	right *= 0.1f;
	up *= 0.5f;

	float3 vertPos[3];
	vertPos[0] = input[0].position.xyz - (right / 2);
	vertPos[1] = input[0].position.xyz + (right / 2);
	vertPos[2] = (up * 2.0f) + input[0].position.xyz +sin(input[0].position.z * input[0].position.x + time);


	// Append plane tri

	output.worldPosition = float4(mul(input[0].position, worldMatrix).xyz, 1.0f);
	output.position = mul(input[0].position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex;
	output.normal = input[0].normal;
	triStream.Append(output);
	
	output.worldPosition = float4(mul(input[1].position, worldMatrix).xyz, 1.0f);
	output.position = mul(input[1].position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[1].tex;
	output.normal = input[1].normal;
	triStream.Append(output);

	output.worldPosition = float4(mul(input[2].position, worldMatrix).xyz, 1.0f);
	output.position = mul(input[2].position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[2].tex;
	output.normal = input[2].normal;
	triStream.Append(output);

	triStream.RestartStrip();
	  
	for (int i = 0; i < 3; i++)
	{
	      float4 vposition = float4(vertPos[i], 1.0f); //+input[0].position;
	      output.worldPosition = float4(mul(vposition, worldMatrix).xyz, 0.0f);
	      output.position = mul(vposition, worldMatrix);
	      output.position = mul(output.position, viewMatrix);
	      output.position = mul(output.position, projectionMatrix);
	      output.tex = g_positions[i];
	      float3 tempNormal = input[0].normal;
	      output.normal = input[0].normal;
	      triStream.Append(output);
	}

	triStream.RestartStrip();
}