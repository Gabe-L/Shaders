#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class boxBlurShader : public BaseShader
{
private:
	struct ResBufferType
	{
		XMFLOAT2 resolution;
		XMFLOAT2 padding;
	};

public:
	boxBlurShader(ID3D11Device* device, HWND hwnd);
	~boxBlurShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, XMFLOAT2 resolution);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11Buffer * resBuffer;
	ID3D11SamplerState* sampleState;
};