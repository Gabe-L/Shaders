Texture2D texture0 : register(t0);
Texture2D grassTex : register(t1);
Texture2D depthTex : register(t2);

SamplerState Sampler0 : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 direction;
    float4 position;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 worldPosition : TEXCOORD1;
    float4 lightViewPos : TEXCOORD2;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(ldiffuse * intensity);
    return colour;
}

struct PS_OUTPUT
{
    float4 colour : SV_Target0;
    float4 cameraDepth : SV_Target1;
};

float4 main(InputType input) : SV_TARGET
{

 //   PS_OUTPUT output;

 //   if (input.worldPosition.w == 0.0f)
 //   {
 //       output.colour = grassTex.Sample(Sampler0, input.tex);
 //   }
 //   else
 //   {
 //       float xRamp = input.position.y / 2.0f;

 //       float4 textureColor = grassTex.Sample(Sampler0, input.tex);
 //       float4 lightColour = 0.f;

 //       float3 lightVector = position.xyz;
 //       lightVector = normalize(lightVector);

 //       lightColour += calculateLighting(-lightVector, input.normal, diffuse);
 //       lightColour += ambient;


 //       output.colour = saturate(lightColour) * textureColor;
 //   }

 //   float depthValue;
	//// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
 //   depthValue = input.position.z / input.position.w;
    
 //   output.cameraDepth = float4(depthValue, depthValue, depthValue, 1.0f);

 //   return output.colour;

    float depthValue;
    float lightDepthValue;
    float shadowMapBias = 0.005f;
    float4 colour = float4(0.f, 0.f, 0.f, 1.f);
    float4 textureColour = texture0.Sample(Sampler0, input.tex);
    int lit = 0;

    float2 pTexCoords = input.lightViewPos.xy / input.lightViewPos.w;
    pTexCoords *= float2(0.5, -0.5);
    pTexCoords += float2(0.5f, 0.5f);

		// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    if (!(pTexCoords.x < 0.f || pTexCoords.x > 1.f || pTexCoords.y < 0.f || pTexCoords.y > 1.f))
    {
		// Sample the shadow map (get depth of geometry)
        depthValue = depthTex.Sample(shadowSampler, pTexCoords).r;

			// Calculate the depth from the light.
        lightDepthValue = input.lightViewPos.z / input.lightViewPos.w;
        lightDepthValue -= shadowMapBias;

			// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        if (lightDepthValue < depthValue)
        {
            colour += calculateLighting(direction.xyz, input.normal, diffuse[2]);
				// Break out so multiple light values aren't given by one point light
            lit = 1;
        }

    }

    if (lit == 0)
    {
        return ambient[0] * textureColour;
    }
    else
    {
        colour = saturate(colour + ambient[0]);
        return colour * textureColour;
    }

}