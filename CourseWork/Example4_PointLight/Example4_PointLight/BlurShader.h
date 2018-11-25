// blur shader handler
// Loads blur shaders (vs and ps)
// Passes screen dimension to shaders, for sample coordinate calculation
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

class BlurShader : public BaseShader
{
private:
	struct ScreenSizeBufferType
	{
		float screenDimension;
		float horScale;
		float verScale;
		float padding;
	};

public:

	BlurShader(ID3D11Device* device, HWND hwnd);
	~BlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float width, XMFLOAT2 scale);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenSizeBuffer;
};