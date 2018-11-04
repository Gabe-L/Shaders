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
    float time;
    float3 padding;
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
    float2 disp : TEXCOORD2;
};

OutputType main(InputType input)
{
	OutputType output;

    float4 textureColour = texture0.SampleLevel(sampler0, input.tex, 0);

    float displacement = 0.0f;
    
    float timeFactor = sin((time / 3) * (2 * 3.141f)) * 0.5f + 0.75f;


    // Broken version
    displacement += sin(textureColour.r);
    displacement += sin(textureColour.g);
    displacement += sin(textureColour.b);

    displacement *= timeFactor;

    // Working meh version
    //displacement += textureColour.r;
    //displacement += textureColour.g;
    //displacement += textureColour.b;

    displacement /= 3;

    input.position.xyz += input.normal * displacement;

    output.disp.x = displacement;
    output.disp.y = 0.5f;

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

	return output;
}