
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix spotProjectionMatrix;
    matrix spotViewMatrix;
    matrix explosionProjectionMatrix;
    matrix explosionViewMatrices[6];
};

cbuffer CamBuffer : register(b1)
{
    float4 camPos;
    float time;
    float3 windPos;
}

cbuffer TexCoordBuffer
{
    static float3 texCoords[3] =
    {
        float3(0, 0.5f, 0),
		float3(0, 0.5f, 0),
		float3(1, 0.5f, 0)
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
    float4 worldPosition : TEXCOORD1; // W component used to determine which texture to use
    float4 depthPosition : TEXCOORD2;
    float4 spotViewPos : TEXCOORD3;
    float4 explosionViewPos[6] : TEXCOORD4;
};

[maxvertexcount(12)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
    OutputType output;

	// Append plane tri
    float3 vertexNormal = normalize(cross(input[0].position.xyz - input[1].position.xyz, input[0].position.xyz - input[2].position.xyz));

    for (int i = 0; i < 3; i++)
    {
        output.worldPosition = float4(mul(input[i].position, worldMatrix).xyz, 1.0f);
        output.position = mul(input[i].position, worldMatrix);
        output.position = mul(output.position, viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        output.depthPosition = output.position;

        output.spotViewPos = mul(input[i].position, worldMatrix);
        output.spotViewPos = mul(output.spotViewPos, spotViewMatrix);
        output.spotViewPos = mul(output.spotViewPos, spotProjectionMatrix);

         // Calculate the position of the vertice as viewed by the light source.
        for (int j = 0; j < 6; j++)
        {
            output.explosionViewPos[j] = mul(input[i].position, worldMatrix);
            output.explosionViewPos[j] = mul(output.explosionViewPos[j], explosionViewMatrices[j]);
            output.explosionViewPos[j] = mul(output.explosionViewPos[j], explosionProjectionMatrix);
        }

        output.tex = input[i].tex;
        output.normal = vertexNormal;
        triStream.Append(output);
    }

    triStream.RestartStrip();
    for (int h = 0; h < 3; h++)
    {
        // Make blade of grass

        float3 forward = normalize(camPos.xyz - input[0].position.xyz);
        float3 up = float3(0, 1, 0);
        float3 right = cross(forward, up);
        up = cross(right, forward);

        right *= 0.1f;
        up *= 0.5f;

        float3 windVec = normalize(windPos - input[0].position.xyz);
        float distanceToWind = sqrt(pow(windPos.x - input[0].position.x, 2) + pow(windPos.y - input[0].position.y, 2));

        // Only keep distance value between 0 and 10
        distanceToWind -= 10.0f;
        distanceToWind = min(distanceToWind, 0.0f);
        distanceToWind *= -1.0f;
        distanceToWind /= 10.0f;

        float3 vertPos[3];
        vertPos[0] = input[0].position.xyz - (right / 2);
        vertPos[1] = input[0].position.xyz + (right / 2);
        //vertPos[2] = (up * 2.0f) + input[0].position.xyz + sin(input[0].position.z * input[0].position.x + time);
        float vertY;
        vertPos[2] = (up * 2.0f) + input[0].position.xyz + (float(h + 1) / 10) * sin(input[0].position.z * input[0].position.x + time);
        vertY = vertPos[2].y;
        vertPos[2] += distanceToWind * -windVec;
        //vertPos[2].y = vertY;

        for (int i = 0; i < 3; i++)
        {
            float4 vposition = float4(vertPos[i], 1.0f);
            output.worldPosition = float4(mul(vposition, worldMatrix).xyz, 0.0f);
            output.position = mul(vposition, worldMatrix);
            output.position = mul(output.position, viewMatrix);
            output.position = mul(output.position, projectionMatrix);

            output.depthPosition = output.position;

            output.spotViewPos = mul(float4(vertPos[i], 1.0f), worldMatrix);
            output.spotViewPos = mul(output.spotViewPos, spotViewMatrix);
            output.spotViewPos = mul(output.spotViewPos, spotProjectionMatrix);

            // Calculate the position of the vertice as viewed by the light source.
            for (int j = 0; j < 6; j++)
            {
                output.explosionViewPos[j] = mul(float4(vertPos[i], 1.0f), worldMatrix);
                output.explosionViewPos[j] = mul(output.explosionViewPos[j], explosionViewMatrices[j]);
                output.explosionViewPos[j] = mul(output.explosionViewPos[j], explosionProjectionMatrix);
            }

            output.tex = texCoords[i];
            output.normal = -forward;
            triStream.Append(output);
        }

        triStream.RestartStrip();
    }
}