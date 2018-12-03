// Tessellation Hull Shader
// Prepares control points for tessellation

cbuffer TessellationBuffer : register(b0)
{
    float tessFactor;
    float3 cameraPosition;
}

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
	float tessFactor : TEXCOORD1;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputType output;

    // Dynamically tesselated from centre of patch
    float3 avgPos = inputPatch[0].position;
    avgPos += inputPatch[1].position;
    avgPos += inputPatch[2].position;
    avgPos += inputPatch[3].position;

    avgPos /= 4;

    // Gets planar distance of camera
    float distance = pow(cameraPosition.x - avgPos.x, 2) + pow(cameraPosition.z - avgPos.z, 2);
    distance = sqrt(distance);

	float tessOut = tessFactor * (1 - (log10(distance / 250) + 1));

    // Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = tessOut;
    output.edges[1] = tessOut;
    output.edges[2] = tessOut;
    output.edges[3] = tessOut;

    // Set the tessellation factor for tessallating inside the triangle.
    output.inside[1] = tessOut;
    output.inside[0] = tessOut;

    return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

	float3 avgPos = patch[0].position;
	avgPos += patch[1].position;
	avgPos += patch[2].position;
	avgPos += patch[3].position;

	avgPos /= 4;

	// Gets planar distance of camera
	float distance = pow(cameraPosition.x - avgPos.x, 2) + pow(cameraPosition.z - avgPos.z, 2);
	distance = sqrt(distance);

	output.tessFactor = tessFactor * (1 - (log(distance / 250) + 1));

    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input texture coordinate as the output texture coordinate.
    output.tex = patch[pointId].tex;

    output.normal = patch[pointId].normal;

    return output;
}