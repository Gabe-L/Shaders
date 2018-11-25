Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
    float screenHeight;
    float scale;
    float2 padding;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;  
};

float4 main(InputType input) : SV_TARGET
{
	float weight[10];
    float weight0, weight1, weight2, weight3, weight4;
    float4 colour;

	// Create the weights that each neighbor pixel will contribute to the blur.
    //weight0 = 0.382928;// * scale;
    //weight1 = 0.241732;// * scale;
    //weight2 = 0.060598;// * scale;
    //weight3 = 0.005977;// * scale;
    //weight4 = 0.000229;// * scale;

    //weight0 = 0.15f;
    //weight1 = 0.15f;
    //weight2 = 0.1f;
    //weight3 = 0.05f;
    //weight4 = 0.05f;

	weight[0] = 0.35f;
	float remainder = (1.0f - weight[0]);
    remainder /= 2.0f;

	for (int i = 1; i < 10; i++) {
        weight[i] = remainder / 2.0f;
		remainder -= weight[i];
	}

	weight[0] += remainder;

    //float remainder = 1.f - (weight0 + 2 * (weight1 + weight2 + weight3 + weight4));

    // Initialize the colour to black.
    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float texelSize = 1.0f / screenHeight;
    // Add the vertical pixels to the colour by the specific weight of each.

    colour += shaderTexture.Sample(SampleType, input.tex) * weight[0];

    for (int j = 1; j < 10; j++)
    {
        colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -j)) * weight[j];
        colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * j)) * weight[j];
    }

    //colour += shaderTexture.Sample(SampleType, input.tex) * remainder;

    //colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -9.0f)) * weight[9];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -8.0f)) * weight[8];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -7.0f)) * weight[7];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -6.0f)) * weight[6];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -5.0f)) * weight[5];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -4.0f)) * weight[4];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -3.0f)) * weight[3];
    //colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -2.0f)) * weight[2];
    //colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * -1.0f)) * weight[1];
    //colour += shaderTexture.Sample(SampleType, input.tex) * weight0;
    //colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 1.0f)) * weight[1];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 2.0f)) * weight[2];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 3.0f)) * weight[3];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 4.0f)) * weight[4];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 5.0f)) * weight[5];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 6.0f)) * weight[6];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 7.0f)) * weight[7];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 8.0f)) * weight[8];
	//colour += shaderTexture.Sample(SampleType, input.tex + float2(0.0f, texelSize * 9.0f)) * weight[9];
 
    //colour += shaderTexture.Sample(SampleType, input.tex) * remainder;


    // Set the alpha channel to one.
    colour.a = 1.0f;

    return saturate(colour);
}

