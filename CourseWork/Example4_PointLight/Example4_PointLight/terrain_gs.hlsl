
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
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
    float4 explosionViewPos[6] : TEXCOORD2;
};

[maxvertexcount(6)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
    OutputType output;

	// Append plane tri
    output.worldPosition = float4(mul(input[0].position, worldMatrix).xyz, 1.0f);
    output.position = mul(input[0].position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < 6; i++)
    {
        output.explosionViewPos[i] = mul(input[0].position, worldMatrix);
        output.explosionViewPos[i] = mul(output.explosionViewPos[i], explosionViewMatrices[i]);
        output.explosionViewPos[i] = mul(output.explosionViewPos[i], explosionProjectionMatrix);
    }

    output.tex = input[0].tex;
    output.normal = input[0].normal;
    triStream.Append(output);
	
    output.worldPosition = float4(mul(input[1].position, worldMatrix).xyz, 1.0f);
    output.position = mul(input[1].position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Calculate the position of the vertice as viewed by the light source.
    for (int j = 0; j < 6; j++)
    {
        output.explosionViewPos[j] = mul(input[1].position, worldMatrix);
        output.explosionViewPos[j] = mul(output.explosionViewPos[j], explosionViewMatrices[j]);
        output.explosionViewPos[j] = mul(output.explosionViewPos[j], explosionProjectionMatrix);
    }
    
    output.tex = input[1].tex;
    output.normal = input[1].normal;
    triStream.Append(output);

    output.worldPosition = float4(mul(input[2].position, worldMatrix).xyz, 1.0f);
    output.position = mul(input[2].position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Calculate the position of the vertice as viewed by the light source.
    for (int i = 0; i < 6; i++)
    {
        output.explosionViewPos[i] = mul(input[2].position, worldMatrix);
        output.explosionViewPos[i] = mul(output.explosionViewPos[i], explosionViewMatrices[i]);
        output.explosionViewPos[i] = mul(output.explosionViewPos[i], explosionProjectionMatrix);
    }

    output.tex = input[2].tex;
    output.normal = input[2].normal;
    triStream.Append(output);

    triStream.RestartStrip();
	  
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
    vertPos[2] = (up * 2.0f) + input[0].position.xyz;
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
        output.tex = texCoords[i];
        output.normal = forward;
        triStream.Append(output);
    }

    triStream.RestartStrip();
}