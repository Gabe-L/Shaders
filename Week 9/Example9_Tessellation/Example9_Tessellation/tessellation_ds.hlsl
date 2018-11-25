// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

Texture2D heightMap : register(t0);
SamplerState Sampler0 : register(s0);

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

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float4 position : POSITION;
    //float4 colour : COLOR;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : POSITION;
    //float4 colour : COLOR;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    //float3 worldPosition : TEXCOORD1;
};

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
	float4 vertexPosition;
	float3 vertexNormal;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).

    // For triangles
    //vertexPosition = uvwCoord.x * patch[0].position + -uvwCoord.y * patch[1].position + -uvwCoord.z * patch[2].position;
	
	// Calculate tex coords
	float2 t1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.y);
	float2 t2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.y);

	float2 texCoord = lerp(t1, t2, uvwCoord.x);

	output.tex = texCoord;

    // For quads
    float4 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
    float4 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
    
    vertexPosition = lerp(v1, v2, uvwCoord.x);

    vertexNormal = cross(patch[0].position.xyz - patch[1].position.xyz, patch[0].position.xyz - patch[2].position.xyz);
    vertexNormal = normalize(vertexNormal);


	//offset position based on sine wave
    //vertexPosition.y = height * (sin((vertexPosition.x * freq) + (time * speed)));

	float4 colour = heightMap.SampleLevel(Sampler0, texCoord, 0);
	vertexPosition.y = 10 * colour.r;

	//modify normals
    //vertexNormal.x = 1 - (height * cos((vertexPosition.x * freq) + (time * speed)));
    //vertexNormal.y = abs(height * cos((vertexPosition.x * freq) + (time * speed)));

	output.position = vertexPosition;
    // Calculate the position of the new vertex against the world, view, and projection matrices.
    //output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    //output.position = mul(output.position, viewMatrix);
    //output.position = mul(output.position, projectionMatrix);

	// Calculate the normal vector against the world matrix only and normalise.
	
	output.normal = vertexNormal;
	//output.normal = mul(vertexNormal, (float3x3)worldMatrix);
	//output.normal = normalize(output.normal);

    // Send the input color into the pixel shader.
    //output.colour = patch[0].colour;

    //output.worldPosition = mul(vertexPosition, worldMatrix).xyz;

    return output;
}

