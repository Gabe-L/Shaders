// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float4 cameraPosition;
}

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 camPosition : SV_POSITION;
    float2 tex : TEXCOORD0;
};

OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.camPosition = mul(cameraPosition, worldMatrix);
    output.camPosition = mul(output.camPosition, viewMatrix);
    output.camPosition = mul(output.camPosition, projectionMatrix);

    //converting to screenspace
    output.camPosition.xyz /= output.camPosition.w;
    output.camPosition.xy *= float2(0.5, -0.5);
    output.camPosition.xy += 0.5f;
    output.camPosition.xy *= float2(1184, 636); //Magic Number, fix this

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    return output;
}