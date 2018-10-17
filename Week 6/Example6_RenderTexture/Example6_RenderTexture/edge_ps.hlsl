// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
    float4 pixel_matrix[3][3];

    float x_total = 0;
    float y_total = 0;
    float2 pixelSize = 1.0f / float2(1184 / 4, 636 / 4);
    float2 offSet = float2(pixelSize.x, pixelSize.y);

    int kernelX[3][3] =
    { // Horizontal Comparison Kernel
        -1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
    };

    int kernelY[3][3] =
    { // Vertical Comparison Kernel
        -1, -2, -1,
		0, 0, 0,
		1, 2, 1
    };


    //fill the pixel matrix with adjacent colors and convert to greyscale
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            pixel_matrix[x][y] = texture0.Sample(sampler0, input.tex + float2(pixelSize.x * (x - 1), pixelSize.y * (y - 1)));
            float temp = pixel_matrix[x][y].x + pixel_matrix[x][y].y + pixel_matrix[x][y].z;
            temp /= 3;
            pixel_matrix[x][y].x = temp;
            pixel_matrix[x][y].y = temp;
            pixel_matrix[x][y].z = temp;
        }
    }


    for (int k_x = 0; k_x < 3; k_x++)
    {
        for (int k_y = 0; k_y < 3; k_y++)
        { // perform kernel convolution on the pixel matrix
            x_total += pixel_matrix[k_x][k_y].x * kernelX[k_x][k_y];
            y_total += pixel_matrix[k_x][k_y].x * kernelY[k_x][k_y];
        }
    }

    float intensity = sqrt(pow(x_total, 2) + pow(y_total, 2)); // find average intensity value between hor and ver difference in intensity

    float4 pixelColour = texture0.Sample(sampler0, input.tex);

    pixelColour.x = intensity;
    pixelColour.y = intensity;
    pixelColour.z = intensity;
    pixelColour.w = 1.0f;

    intensity /= 1141;
	//intensity *= 255;

	//greyscale code
    //float grey = (pixelColour.r + pixelColour.g + pixelColour.b) / 3;
    //pixelColour = float4(grey, grey, grey, grey);

    return pixelColour;
}