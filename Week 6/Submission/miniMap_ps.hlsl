// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float2 cameraPos : TEXCOORD1;
    float2 resolution : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
    float4 pixelColour = texture0.Sample(sampler0, input.tex);
   
    // Convert resolution to UV space
    float2 pixelSize = 1.0f / input.resolution;
    float2 offSet = float2(pixelSize.x, pixelSize.y); //could change this value for greater offset

    float dist = sqrt(pow(((input.cameraPos.x / 50) * pixelSize.x) - input.tex.x, 2) + pow(((input.cameraPos.y / 50) * pixelSize.y) - input.tex.y, 2));

   if (dist < 0.03)
   {
       return float4(1, 1, 0, 1); // Player's location within range, change colour
   }

    return pixelColour;

}