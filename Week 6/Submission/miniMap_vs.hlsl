// MiniMap vertex shader

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float4 camPosition;
    float2 res;
    float2 padding;
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
    float2 cameraPos : TEXCOORD1;
    float2 resolution : TEXCOORD2;
};

OutputType main(InputType input)
{
    OutputType output;

    output.tex = input.tex;
    output.resolution = res;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate normal
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    output.normal = input.normal;
    
    // Create local camera info to allow modification
    float4 camInfo = camPosition;

    // Flip values to use x-z plane instead of x-y plane
    float temp = camInfo.y;
    camInfo.y = -camInfo.z;
    camInfo.z = temp;

    // Convert camera position to screenspace
    camInfo.xyz /= camInfo.w;
    camInfo.xy *= float2(0.5f, -0.5f);
    camInfo.xy += 0.5f;
    camInfo.xy *= res;

    output.cameraPos = camInfo.xy;

    return output;
}