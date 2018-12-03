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

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float tessFactor : TEXCOORD1;
};

struct OutputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
    float4 vertexPosition;
    float3 vertexNormal = float3(0,0,0);
    OutputType output;
 
	float fakeTessFactor = 10.0f;

    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	
	// Calculate tex coords
    float2 t1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.y);
    float2 t2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.y);

    float2 texCoord = lerp(t1, t2, uvwCoord.x);


    // Calculate new vertex position
    float4 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
    float4 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
    
    vertexPosition = lerp(v1, v2, uvwCoord.x);

	float2 mods[4] = {
		float2(-1.0f, -1.0f),
		float2(1.0f, -1.0f),
		float2(1.0f, 1.0f),
		float2(-1.0f, 1.0f)
	};

	float2 texelSize = 1.0f / float2(1184, 636);

	for (int i = 0; i < 4; i++) {
		float3 nPos1, nPos2;

		float avgTess = patch[0].tessFactor + patch[1].tessFactor + patch[2].tessFactor + patch[3].tessFactor;
		avgTess /= 4;

		nPos1 = vertexPosition;
		nPos1.x += mods[i].x / avgTess;
		nPos2 = vertexPosition;
		nPos2.z += mods[i].y / avgTess;

		nPos1.y = 10 * heightMap.SampleLevel(Sampler0, texCoord + float2((mods[i].x / avgTess) / 15.0f, 0), 0).r;
		nPos2.y = 10 * heightMap.SampleLevel(Sampler0, texCoord + float2(0, (mods[i].y / avgTess) / 15.0f), 0).r;

		/*nPos1.y = (10 * heightMap.SampleLevel(Sampler0, texCoord, 0).r);
		nPos2.y = (10 * heightMap.SampleLevel(Sampler0, texCoord, 0).r);*/

		if (i == 0 || i == 2) {
			vertexNormal += normalize(cross(vertexPosition - nPos1, vertexPosition - nPos2));
		}
		else
		{
			vertexNormal += normalize(cross(vertexPosition - nPos2, vertexPosition - nPos1));
		}
	}

    // Calculate vertex normal base on patch positions
    //vertexNormal = cross(patch[0].position.xyz - patch[1].position.xyz, vertexPosition - patch[2].position.xyz);

	vertexNormal /= 4;

    // Offset vertex based on height map
    float4 colour = heightMap.SampleLevel(Sampler0, texCoord, 0);
    vertexPosition.y = 10 * colour.r;

    // Set output data
    output.tex = texCoord;
    output.position = vertexPosition;
    output.normal = -normalize(vertexNormal);

    return output;
}

