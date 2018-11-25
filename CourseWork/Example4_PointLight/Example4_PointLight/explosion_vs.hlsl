Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer : register(b1)
{
    float4 displacementFactor;
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
    float2 disp : TEXCOORD2;
    float progress : TEXCOORD3;
};

OutputType main(InputType input)
{
    OutputType output;

    float4 textureColour = texture0.SampleLevel(sampler0, input.tex, 0);

    float displacement = 0.0f;
    
    displacement = textureColour.r * displacementFactor.r + textureColour.g * displacementFactor.g + textureColour.b * displacementFactor.b;

    displacement /= 3;

    float progressOffset = (1.5f * displacementFactor.w);
    input.position.xyz += input.normal * ((displacement * progressOffset) + (-1.0f + progressOffset));

    output.disp.x = displacement;
    output.disp.y = 0.5f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    output.progress = displacementFactor.w;

    return output;
}