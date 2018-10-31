#pragma once

#include "BaseShader.h"

using namespace std;
using namespace DirectX;

class MouseShader : public BaseShader
{
private:
	struct MouseBufferType
	{
		XMFLOAT2 mousePos;
		XMFLOAT2 resolution;
	};

public:
	MouseShader(ID3D11Device* device, HWND hwnd);
	~MouseShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, XMFLOAT2 mousePos, XMFLOAT2 resolution);

private:
	void initShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* mouseBuffer;
};