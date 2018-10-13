// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 main(InputType input) : SV_TARGET
{
	float4 textureColor;

	float4 pixel_matrix[3][3];

	float x_total = 0;
	float y_total = 0;

	int kernelX[3][3] = { // Horizontal Comparison Kernel
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};

	int kernelY[3][3] = { // Vertical Comparison Kernel
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = texture0.Sample(Sampler0, input.tex);

	//fill the pixel matrix with adjacent colors and convert to greyscale
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			pixel_matrix[x][y] = texture0.Sample(Sampler0, input.tex + float2(x, y));
			float temp = pixel_matrix[x][y].x + pixel_matrix[x][y].y + pixel_matrix[x][y].z;
			temp /= 3;
			temp *= 255;
			pixel_matrix[x][y].x = temp;
			pixel_matrix[x][y].y = temp;
			pixel_matrix[x][y].z = temp;
		}
	}

	for (int k_x = 0; k_x < 3; k_x++) {
		for (int k_y = 0; k_y < 3; k_y++) {// perform kernel convolution on the pixel matrix
			x_total += pixel_matrix[k_x][k_y].x * kernelX[k_x][k_y];
			y_total += pixel_matrix[k_x][k_y].x * kernelY[k_x][k_y];
		}
	}


	float intensity = sqrt((x_total * x_total) + (y_total * y_total)); // find average intensity value between hor and ver difference in intensity

	//intensity /= 1141;
	//intensity *= 255;

	//Invert texture color

	//textureColor.x = 1 - textureColor.x;
	//textureColor.y = 1 - textureColor.y;
	//textureColor.z = 1 - textureColor.z;

	textureColor.x = intensity;
	textureColor.y = intensity;
	textureColor.z = intensity;

	/*if (pixel_matrix[0][0].x == pixel_matrix[2][2].x) {
		return float4(0, 0, 0, 0);
	}
	else {
		return float4(1, 1, 1, 1);
	}*/

	return textureColor;
}