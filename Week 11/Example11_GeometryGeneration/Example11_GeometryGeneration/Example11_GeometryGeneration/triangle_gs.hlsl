
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CamBuffer : register(b1)
{
    float4 camPos;
}

cbuffer PositionBuffer
{
    static float3 g_positions[4] =
    {
        float3(0, 0, 0),
        float3(0, 1, 0),
        float3(1, 0, 0),
        float3(1, 1, 0)
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

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	float3 forward = normalize(camPos.xyz - input[0].position.xyz);
    float3 up = float3(0, 1, 0);
    float3 right = cross(forward, up);
    up = cross(right, forward);

    right *= 5.0f;
    up *= 5.0f;

    float3 vertPos[4];
    vertPos[0] = up + input[0].position.xyz;
    vertPos[1] = input[0].position.xyz;
    vertPos[2] = up + right + input[0].position.xyz;
    vertPos[3] = right + input[0].position.xyz;

   for (int i = 0; i < 4; i++)
    {
		//float4 vposition = float4(cross(float4(g_positions[i], 1.0f), float4(camDirection, 1.0f)), 1.0f);
        float4 vposition = float4(vertPos[i], 1.0f);//+input[0].position;
        output.worldPosition = mul(vposition, worldMatrix).xyz;
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