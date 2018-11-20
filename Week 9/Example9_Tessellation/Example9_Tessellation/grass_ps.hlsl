Texture2D texture0 : register(t0);
Texture2D grassTex : register(t1);
SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 position;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 worldPosition : TEXCOORD1;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(ldiffuse * intensity);
    return colour;
}

float4 main(InputType input) : SV_TARGET
{

	if (input.worldPosition.w == 0.0f) {
		return grassTex.Sample(Sampler0, input.tex);
	}

    float xRamp = input.position.y / 2.0f;

    //float4 textureColor = texture0.Sample(Sampler0, float2(xRamp, 0.5f));
    float4 textureColor = texture0.Sample(Sampler0, input.tex);
    float4 lightColour = 0.f;

    float3 lightVector = position.xyz - input.worldPosition;
    lightVector = normalize(lightVector);

    lightColour += calculateLighting(-lightVector, input.normal, diffuse);

    lightColour += ambient;

    //return float4(0, 0.4, 0, 1);
	return textureColor;
    return saturate(lightColour) * textureColor;
}