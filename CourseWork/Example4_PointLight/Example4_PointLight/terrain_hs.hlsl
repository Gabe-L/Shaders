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

    //if (distance < 50.0f)
    {
        // Set the tessellation factors for the three edges of the triangle.
        output.edges[0] = tessFactor * (1 - (log(distance / 250) + 1));
        output.edges[1] = tessFactor * (1 - (log(distance / 250) + 1));
        output.edges[2] = tessFactor * (1 - (log(distance / 250) + 1));
        output.edges[3] = tessFactor * (1 - (log(distance / 250) + 1));

    // Set the tessellation factor for tessallating inside the triangle.
        output.inside[1] = tessFactor * (1 - (log(distance / 250) + 1));
        output.inside[0] = tessFactor * (1 - (log(distance / 250) + 1));
    }
    //else
    //{
    //// Set the tessellation factors for the three edges of the triangle.
    //    output.edges[0] = tessFactor / 2;
    //    output.edges[1] = tessFactor / 2;
    //    output.edges[2] = tessFactor / 2;
    //    output.edges[3] = tessFactor / 2;

    //// Set the tessellation factor for tessallating inside the triangle.
    //    output.inside[1] = tessFactor / 2;
    //    output.inside[0] = tessFactor / 2;
    //}

    return output;
}


[domain("quad")]
[partitioning("fractional_odd")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input texture coordinate as the output texture coordinate.
    output.tex = patch[pointId].tex;

    output.normal = patch[pointId].normal;

    return output;
}