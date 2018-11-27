
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix;
	matrix lightProjectionMatrix;
    matrix lightViewMatrixTwo;
    matrix lightProjectionMatrixTwo;
	matrix lightViews[6];
	matrix lightProjections[6];
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
    float4 lightViewPos : TEXCOORD1;
    float4 lightViewPos2 : TEXCOORD2;
	float3 worldPosition : TEXCOORD3;
	float4 lightViewsPos[6] : TEXCOORD4;
};


OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.worldPosition = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the position of the vertice as viewed by the light source.
    output.lightViewPos = mul(input.position, worldMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightViewMatrix);
    output.lightViewPos = mul(output.lightViewPos, lightProjectionMatrix);

    // Calculate the position of the vertice as viewed by the second light source.
    output.lightViewPos2 = mul(input.position, worldMatrix);
    output.lightViewPos2 = mul(output.lightViewPos2, lightViewMatrixTwo);
    output.lightViewPos2 = mul(output.lightViewPos2, lightProjectionMatrixTwo);

	for (int i = 0; i < 6; i++) {
		output.lightViewsPos[i] = mul(input.position, worldMatrix);
		output.lightViewsPos[i] = mul(output.lightViewsPos[i], lightViews[i]);
		output.lightViewsPos[i] = mul(output.lightViewsPos[i], lightProjections[i]);
	}

    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}