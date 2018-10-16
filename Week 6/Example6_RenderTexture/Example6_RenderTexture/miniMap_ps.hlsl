// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
    float4 camPosition : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour = texture0.Sample(sampler0, input.tex);
    
    //if (input.tex.x == input.camPosition.x && input.tex.x == input.camPosition.x)
    //{
    //    textureColour = float4(1.0f, 0.0f, 0.0f, 0.0f);
    //}

    float xDiff = input.tex.x - input.camPosition.x;
    float yDiff = input.tex.y - input.camPosition.y;

    //textureColour += float4(1.0f, 0.0f, 0.0f, 0.0f);
    return float4(1, 1, 1, 1);
    return textureColour;
}



